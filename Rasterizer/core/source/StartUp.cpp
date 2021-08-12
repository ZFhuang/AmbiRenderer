#include <utility>
#include <cstdlib>
#include <string>
#include <limits>
#include <vector>
#include <cmath>

#include "../include/tgaimage.h"
#include "../include/model.h"
#include "../include/gl_soft.h"
#include "../../shaders/v_Basic.cpp"
#include "../../shaders/f_Phong.cpp"
#include "../../shaders/f_Gouraud.cpp"
#include "../../shaders/f_Stylized.cpp"
#include "../../shaders/f_Depth.cpp"
#include "../../shaders/f_Normal.cpp"

namespace PATH
{
	const std::string RESOURCES = "C:/Work/AmbiRenderer/Resources/";
}

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

Matrix modelViewMat;
Matrix projMat;
Matrix viewportMat;

int width = 800;
int height = 800;
int depth = 255;
Vec3f light_dir(1, 1, 0);
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);
Vec3f up(0, 1, 0);

struct RenderArgs
{
	RenderArgs(TGAImage* f_Buffer, Model* m, v_Shader* v_s, f_Shader* f_s) :
		frameBuffer(f_Buffer), model(m), v_shader(v_s), f_shader(f_s) {}

	~RenderArgs() {
		delete frameBuffer;
		delete f_shader;
		delete v_shader;
	}

	TGAImage* frameBuffer;
	Model* model;
	v_Shader* v_shader;
	f_Shader* f_shader;
};

std::vector<std::vector<float>> triangleTraversal(TGAImage* frameBuffer, std::vector<std::vector<float>>& v_out) {
	std::vector<std::vector<float>> f_in;

	// 生成三角形的二维包围盒
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(frameBuffer->get_width() - 1, frameBuffer->get_height() - 1);
	Vec3f pts[3];
	int n_faces = v_out.size() / 3;

	for (int i = 0; i < n_faces; i++) {
		for (int j = 0; j < 3; j++) {
			pts[j] = Vec3f(v_out[i * 3 + j][0], v_out[i * 3 + j][1], v_out[i * 3 + j][2]);
			for (int k = 0; k < 2; k++) {
				bboxmin[k] = std::max(0.0f, std::min(bboxmin[k], pts[j][k]));
				bboxmax[k] = std::min(clamp[k], std::max(bboxmax[k], pts[j][k]));
			}
		}

		// 光栅化
		Vec3f P;
		for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
			for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
				// 得到包围盒内像素的重心坐标判断是否在三角形内
				Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
				if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
					continue;
				}

				// 插值目标像素的z值
				P.z = 0;
				for (int j = 0; j < 3; j++) P.z += pts[j][2] * bc_screen[j];

				std::vector<float> fragment = { float(i), float(P.x), float(P.y), float(P.z), bc_screen.x, bc_screen.y, bc_screen.z };

				f_in.push_back(fragment);
			}
		}
	}

	return f_in;
}

std::vector<std::vector<float>> vertexShading(Model* model, v_Shader* v_shader) {
	std::vector<std::vector<float>> v_out;

	for (int i = 0; i < model->nfaces(); i++) {
		for (int j = 0; j < 3; j++) {
			v_out.push_back(v_shader->vertex(i, model->face(i)[j * 2]));
		}
	}

	return v_out;
}

std::vector<std::vector<float>> fragmentShading(std::vector<std::vector<float>> f_in, f_Shader* f_shader) {
	std::vector<std::vector<float>> f_out;

	int n_fragments = f_in.size();

	for (int i = 0; i < n_fragments; i++) {
		f_out.push_back(f_shader->fragment(f_in[i]));
	}

	return f_out;
}

void depthTestAndBlend(TGAImage* frameBuffer, std::vector<std::vector<float>> f_out) {
	int** zBuffer = makeZBuffer(frameBuffer->get_height(), frameBuffer->get_width());
	int n_fragments = f_out.size();

	for (int i = 0; i < n_fragments; i++) {
		std::vector<float> f = f_out[i];

		int x = f[0];
		int y = f[1];
		int z = f[2];

		if (zBuffer[x][y] < z) {
			// BGRA
			frameBuffer->set(x, y, TGAColor(f[5], f[4], f[3], f[6]));
			zBuffer[x][y] = z;
		}
	}

	deleteZBuffer(zBuffer, frameBuffer->get_height());
}

RenderArgs* makeShadowArgs(Model* model) {
	int shadow_width = width / 2;
	int shadow_height = height / 2;

	TGAImage* shadow_buffer = new TGAImage(shadow_width, shadow_height, TGAImage::RGB);
	modelViewMat = makeViewMat(light_dir, center, Vec3f(0, 1, 0));
	viewportMat = makeViewportMat(shadow_width / 8, shadow_height / 8, shadow_width * 3 / 4, shadow_height * 3 / 4);
	projMat = makeProjMat(0);

	Matrix* all_mat_vertex = new Matrix(viewportMat * projMat * modelViewMat);

	v_Basic* v_shader = new v_Basic();
	v_shader->mat = all_mat_vertex;
	v_shader->model = model;

	f_Depth* f_shader = new f_Depth();

	RenderArgs* shadowArgs = new RenderArgs(shadow_buffer, model, v_shader, f_shader);
	return shadowArgs;
}

RenderArgs* makeMainArgs(Model* model, TGAImage* diffuse, TGAImage* specular, TGAImage* normalMap, TGAImage* shadowBuffer) {
	TGAImage* frameBuffer = new TGAImage(width, height, TGAImage::RGB);

	modelViewMat = makeViewMat(eye, center, Vec3f(0, 1, 0));
	viewportMat = makeViewportMat(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	projMat = makeProjMat((eye - center).norm());

	Matrix* all_mat_vertex = new Matrix(viewportMat * projMat * modelViewMat);
	Matrix all_mat = projMat * modelViewMat;
	Matrix* all_mat_invert_transpose = new Matrix(all_mat.invert_transpose());
	Vec4f lightDir_4 = all_mat * Vec4f(light_dir.x, light_dir.y, light_dir.z, 0.0f);
	light_dir = Vec3f(lightDir_4.x, lightDir_4.y, lightDir_4.z);

	v_Basic* v_shader = new v_Basic();
	v_shader->mat = all_mat_vertex;
	v_shader->model = model;

	f_Phong* f_shader = new f_Phong();
	f_shader->mat_invert_transpose = all_mat_invert_transpose;
	f_shader->lightDir = new Vec3f(light_dir.normalize());
	f_shader->viewDir = new Vec3f((eye - center).normalize());
	f_shader->model = model;
	f_shader->diffuse = diffuse;
	f_shader->specular = specular;
	f_shader->normalMap = normalMap;

	RenderArgs* mainArgs = new RenderArgs(frameBuffer, model, v_shader, f_shader);
	return mainArgs;
}

void render(RenderArgs* args) {
	std::vector<std::vector<float>> v_out = std::move(vertexShading(args->model, args->v_shader));
	std::vector<std::vector<float>> f_in = std::move(triangleTraversal(args->frameBuffer, v_out));
	std::vector<std::vector<float>> f_out = std::move(fragmentShading(f_in, args->f_shader));
	depthTestAndBlend(args->frameBuffer, f_out);
	args->frameBuffer->flip_vertically();
}

void multipassRender(Model* model, TGAImage* diffuse = nullptr, TGAImage* specular = nullptr, TGAImage* normalMap = nullptr) {
	RenderArgs* shadowArgs = makeShadowArgs(model);
	render(shadowArgs);
	shadowArgs->frameBuffer->write_tga_file("shadow.tga");

	RenderArgs* mainArgs = makeMainArgs(model, diffuse, specular, normalMap, shadowArgs->frameBuffer);
	render(mainArgs);
	mainArgs->frameBuffer->write_tga_file("framebuffer.tga");

	delete shadowArgs;
	delete mainArgs;
}

int main(int argc, char** argv) {
	TGAImage* diffuse = new TGAImage();
	TGAImage* specular = new TGAImage();
	TGAImage* normalMap = new TGAImage();
	bool tex_success = diffuse->read_tga_file((PATH::RESOURCES + "african_head_diffuse.tga").c_str());
	assert(tex_success);
	tex_success = specular->read_tga_file((PATH::RESOURCES + "african_head_spec.tga").c_str());
	assert(tex_success);
	tex_success = normalMap->read_tga_file((PATH::RESOURCES + "african_head_nm_tangent.tga").c_str());
	assert(tex_success);
	diffuse->flip_vertically();
	specular->flip_vertically();
	normalMap->flip_vertically();

	Model* model = new Model((PATH::RESOURCES + "african_head.obj").c_str());
	model->initTextureCoord(*diffuse);

	multipassRender(model, diffuse, specular, normalMap);

	delete diffuse;
	delete specular;
	delete normalMap;
	delete model;
	return 0;
}
