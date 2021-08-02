#include"../include/tgaimage.h"
#include"../include/model.h"
#include"../include/gl_soft.h"
#include<utility>
#include<cstdlib>
#include<string>
#include <limits>
#include <vector>
#include <cmath>

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

void triangle(Vec3f* pts, int** zbuffer, TGAImage& image, Vec3f* norms, TGAImage* texture = nullptr, Vec2f* texPts = nullptr) {
	// 绘制三角形
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
	// 获取三角形的二维包围盒
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}
	Vec3f P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			// 只绘制三角形内的像素
			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
				continue;
			}

			// 利用重心坐标插值三个顶点的z值得到目标像素的z值
			P.z = 0;
			for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];

			// 只绘制靠前的像素
			if (zbuffer[int(P.x)][int(P.y)] > P.z) {
				continue;
			}
			zbuffer[int(P.x)][int(P.y)] = P.z;
			TGAColor color = white;

			if (texture != nullptr) {
				// 利用重心坐标插值材质坐标
				Vec2f T(0, 0);
				for (int i = 0; i < 3; i++) {
					T.x += texPts[i].x * bc_screen[i];
					T.y += texPts[i].y * bc_screen[i];
				}
				// 读取对应的材质颜色并与光照加权
				color = texture->get(int(T.x), int(T.y));
			}

			// 利用重心坐标插值三个顶点的法线值
			Vec3f N(0, 0, 0);
			for (int i = 0; i < 3; i++) {
				N.x += norms[i].x * bc_screen[i];
				N.y += norms[i].y * bc_screen[i];
				N.z += norms[i].z * bc_screen[i];
			}

			N.normalize();
			float intensity = N * light_dir.normalize();
			if (intensity > 0) {
				color.r *= intensity;
				color.g *= intensity;
				color.b *= intensity;
				//TGAColor z_color(int((P.z + 1) / 2 * 255), int((P.z + 1) / 2 * 255), int((P.z + 1) / 2 * 255), 255);
				image.set(P.x, P.y, color);
			}
		}
	}
}

void render(Model* model, TGAImage& image, int** zbuffer, TGAImage* texture = nullptr) {
	Matrix all_mat = viewportMat * projMat * modelViewMat;

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);

		Vec3f n = cross(model->vert(face[1 * 2]) - model->vert(face[0 * 2]), model->vert(face[2 * 2]) - model->vert(face[0 * 2]));

		// 矩阵连乘得到屏幕空间的顶点
		Vec3f pts[3];
		for (int i = 0; i < 3; i++) {
			Vec4f v = all_mat * Vec4f(model->vert(face[i * 2]).x, model->vert(face[i * 2]).y, model->vert(face[i * 2]).z, 1.0f);
			pts[i] = Vec3f(v.x / v.w, v.y / v.w, v.z / v.w);
		}

		// 顶点法线值
		Vec3f norms[3];
		if (model->nnorm() != 0) {
			for (int i = 0; i < 3; i++) {
				norms[i] = model->norm(face[i * 2]).normalize();
			}
		}
		else {
			n.normalize();
			for (int i = 0; i < 3; i++) {
				norms[i] = n;
			}
		}

		// 顶点材质坐标
		Vec2f texPts[3];
		if (texture != nullptr) {
			for (int i = 0; i < 3; i++) {
				texPts[i] = model->tex_[face[i * 2 + 1]];
			}
		}

		// 绘制三角形
		triangle(pts, zbuffer, image, norms, texture, texPts);
	}
}


Vec3f calModelCenter(Model* model) {
	Vec3f c = Vec3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < model->nverts(); i++) {
		c = c + model->vert(i);
	}
	c = c / model->nverts();
	return c;
}

int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);
	TGAImage texture;
	bool tex_success = texture.read_tga_file((PATH::RESOURCES + "african_head_diffuse.tga").c_str());
	assert(tex_success);
	texture.flip_vertically();
	Model* model = new Model((PATH::RESOURCES + "african_head.obj").c_str());
	model->initTextureCoord(texture);

	modelViewMat = makeViewMat(eye, center, Vec3f(0, 1, 0));
	viewportMat = makeViewportMat(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	projMat = makeProjMat((eye - center).norm());
	int** zbuffer = makeZBuffer();

	render(model, image, zbuffer, &texture);

	image.flip_vertically();
	image.write_tga_file("output.tga");

	delete model;
	deleteZBuffer(zbuffer);
	return 0;
}
