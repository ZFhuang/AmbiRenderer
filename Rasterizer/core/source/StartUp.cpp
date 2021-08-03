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
Vec3f light_dir(1, 1, 1);
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);
Vec3f up(0, 1, 0);

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
	// 计算三角形ABC中点P对应的重心坐标
	// 任意两边的向量
	Vec3f s[2];
	for (int i = 2; i--; ) {
		s[i][0] = int(C[i]) - int(A[i]);
		s[i][1] = int(B[i]) - int(A[i]);
		s[i][2] = int(A[i]) - int(P[i]);
	}
	// 叉乘得到的第三轴调整后就是重心坐标
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

std::vector<std::vector<float>> triangleTraversal(TGAImage& frameBuffer, std::vector<std::vector<float>>& v_out) {
	std::vector<std::vector<float>> f_in;

	// 生成三角形的二维包围盒
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(frameBuffer.get_width() - 1, frameBuffer.get_height() - 1);
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
				for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];

				std::vector<float> fragment = { float(i),float(P.x), float(P.y), float(P.z), bc_screen.x, bc_screen.y, bc_screen.z };

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

void depthTestAndBlend(TGAImage& frameBuffer, std::vector<std::vector<float>> f_out) {
	int** zBuffer = makeZBuffer(frameBuffer.get_height(), frameBuffer.get_width());
	int n_fragments = f_out.size();

	for (int i = 0; i < n_fragments; i++) {
		std::vector<float> f = f_out[i];

		int x = f[0];
		int y = f[1];
		int z = f[2];

		if (zBuffer[x][y] < z) {
			// BGRA
			frameBuffer.set(x, y, TGAColor(f[5], f[4], f[3], f[6]));
			zBuffer[x][y] = z;
		}
	}

	deleteZBuffer(zBuffer, frameBuffer.get_height());
}

void render(TGAImage& frameBuffer, Model* model, v_Shader* v_shader, f_Shader* f_shader) {
	std::cout << "vertexShading..." << std::endl;
	std::vector<std::vector<float>> v_out = std::move(vertexShading(model, v_shader));
	std::cout << "triangle_traversal..." << std::endl;
	std::vector<std::vector<float>> f_in = std::move(triangleTraversal(frameBuffer, v_out));
	std::cout << "fragmentShading..." << std::endl;
	std::vector<std::vector<float>> f_out = std::move(fragmentShading(f_in, f_shader));
	std::cout << "depthTestAndBlend..." << std::endl;
	depthTestAndBlend(frameBuffer, f_out);
	std::cout << "ok." << std::endl;
}

int main(int argc, char** argv) {
	TGAImage frameBuffer(width, height, TGAImage::RGB);
	TGAImage diffuse;
	TGAImage specular;
	bool tex_success = diffuse.read_tga_file((PATH::RESOURCES + "african_head_diffuse.tga").c_str());
	assert(tex_success);
	tex_success = specular.read_tga_file((PATH::RESOURCES + "african_head_spec.tga").c_str());
	assert(tex_success);
	diffuse.flip_vertically();
	specular.flip_vertically();
	Model* model = new Model((PATH::RESOURCES + "african_head.obj").c_str());
	model->initTextureCoord(diffuse);

	modelViewMat = makeViewMat(eye, center, Vec3f(0, 1, 0));
	viewportMat = makeViewportMat(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	projMat = makeProjMat((eye - center).norm());
	Matrix all_mat = viewportMat * projMat * modelViewMat;

	v_Basic* v_shaer = new v_Basic();
	v_shaer->mat = &all_mat;
	v_shaer->model = model;

	f_Phong* f_shader = new f_Phong();
	f_shader->lightDir = light_dir;
	f_shader->viewDir = eye - center;
	f_shader->model = model;
	f_shader->diffuse = &diffuse;
	f_shader->specular = &specular;

	render(frameBuffer, model, v_shaer, f_shader);

	frameBuffer.flip_vertically();
	frameBuffer.write_tga_file("output.tga");

	delete model;
	delete v_shaer;
	delete f_shader;
	return 0;
}
