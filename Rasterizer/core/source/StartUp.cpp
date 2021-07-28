#include"../include/tgaimage.h"
#include"../include/model.h"
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
const int width = 800;
const int height = 800;

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
	// 计算三角形ABC中点P对应的重心坐标
	// 任意两边的向量
	Vec3f s[2];
	for (int i = 2; i--; ) {
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	// 叉乘得到的第三轴调整后就是重心坐标
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color) {
	int x0 = t0.x;
	int y0 = t0.y;
	int x1 = t1.x;
	int y1 = t1.y;
	// 按照长边遍历
	bool isSteep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		isSteep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	// 从小值遍历到大值
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	// 用长短边的累计误差来替代0.5的浮点检测, 乘2使得分界点变为1
	int yerror = std::abs(y1 - y0) * 2;
	int xerror = x1 - x0;
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		// 由于长边交换, 因此绘制顺序需要换回来
		if (isSteep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error += yerror;
		if (error > xerror) {
			// 两种方向的步进
			y += y0 > y1 ? -1 : 1;
			error -= xerror * 2;
		}
	}
}

void triangle(Vec3f* pts, float** zbuffer, TGAImage& image, TGAColor color) {
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
			// 通过重心坐标判断当前像素在不在三角形内
			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			// 利用重心坐标插值三个顶点的z值得到目标像素的z值
			for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];
			if (zbuffer[int(P.x)][int(P.y)] < P.z) {
				// 在当前的Z缓冲前面时就要进行绘制
				zbuffer[int(P.x)][int(P.y)] = P.z;
				// 如果要渲染Z缓冲就使用下面的颜色
				TGAColor z_color(int((P.z + 1) / 2 * 255), int((P.z + 1) / 2 * 255), int((P.z + 1) / 2 * 255), 255);
				image.set(P.x, P.y, color);
			}
		}
	}
}


void drawWireFrame(TGAImage& image, Model* model) {
	// 遍历模型的每一个面
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		// 遍历面的每一条边
		for (int j = 0; j < 3; j++) {
			// 利用面的两个顶点序号从顶点数组中得到具体的顶点坐标
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			// 样例模型的坐标是归一化过的(-1,1), 因此通过这样的映射令其填充画幅
			int x0 = (v0.x + 1.) * width / 2.;
			int y0 = (v0.y + 1.) * height / 2.;
			int x1 = (v1.x + 1.) * width / 2.;
			int y1 = (v1.y + 1.) * height / 2.;
			// 绘制线
			line(Vec2i(x0, y0), Vec2i(x1, y1), image, white);
		}
	}
}

void rasterize(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color, int ybuffer[]) {
	// 思路和光栅化差不多
	if (p0.x > p1.x) {
		std::swap(p0, p1);
	}
	for (int x = p0.x; x <= p1.x; x++) {
		// 线性插值得到y值
		float t = (x - p0.x) / (float)(p1.x - p0.x);
		int y = p0.y * (1. - t) + p1.y * t;
		// 只保留最大的y(离相机最近)
		if (ybuffer[x] < y) {
			ybuffer[x] = y;
			image.set(x, 0, color);
		}
	}
}

Vec3f world2screen(Vec3f v) {
	// 正交投影的转换方法, 此时的裁剪体是[-1, 1], 改变xy但是不改变z, 反之z只是一个相对值
	return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

float** initZBuffer() {
	// 初始化Z缓冲为最大值
	float** zbuffer = new float* [height];
	for (int line = 0; line < height; line++) {
		zbuffer[line] = new float[width];
	}
	for (int line = 0; line < height; line++) {
		for (int item = 0; item < width; item++) {
			zbuffer[line][item] = -std::numeric_limits<float>::max();
		}
	}
	return zbuffer;
}

void deleteZBuffer(float** zbuffer) {
	for (int line = 0; line < height; line++) {
		delete zbuffer[line];
	}
	delete zbuffer;
}

void render(Model* model, TGAImage& image, Vec3f light_dir, float** zbuffer) {
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		// 计算光照因子
		Vec3f n = cross(model->vert(face[1]) - model->vert(face[0]), model->vert(face[2]) - model->vert(face[0]));
		n.normalize();
		float intensity = -(n * light_dir);
		Vec3f pts[3];
		// 三角形的每个面都需要转到屏幕空间中才能进行Z缓冲渲染
		for (int i = 0; i < 3; i++) pts[i] = world2screen(model->vert(face[i]));
		if (intensity > 0) {
			triangle(pts, zbuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}
	}
}

int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);
	Model* model = new Model((PATH::RESOURCES + "african_head.obj").c_str());
	Vec3f light_dir(0, 0, -1);

	float** zbuffer = initZBuffer();
	render(model, image, light_dir, zbuffer);

	image.flip_vertically();
	image.write_tga_file("output.tga");

	delete model;
	deleteZBuffer(zbuffer);
	return 0;
}
