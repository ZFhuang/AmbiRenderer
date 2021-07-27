#include"../include/tgaimage.h"
#include"../include/model.h"
#include<utility>
#include<cstdlib>
#include<string>

namespace PATH
{
	const std::string RESOURCES = "C:/Work/AmbiRenderer/Resources/";
}

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

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

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
	// 扫描线法绘制三角形
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	// 使用扫描算法来对三角形进行着色
	int total_height = t2.y - t0.y;
	// 按照y进行从下到上扫描
	for (int i = 0; i < total_height; i++) {
		// 用bool标记当前是上段还是下段
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
		Vec2i A = t0 + (t2 - t0) * alpha;
		Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			image.set(j, t0.y + i, color);
		}
	}
}

void drawWireFrame(TGAImage& image, Model* model) {
	// 绘制线框模型
	int width = image.get_width();
	int height = image.get_height();
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

void drawRandomColorModel(TGAImage& image, Model* model) {
	int width = image.get_width();
	int height = image.get_height();
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f world_coords = model->vert(face[j]);
			// 这里将世界坐标中的模型缩放到屏幕中
			screen_coords[j] = Vec2i((world_coords.x + 1.) * width / 2., (world_coords.y + 1.) * height / 2.);
		}
		// 用随机颜色来绘制三角形
		triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}
}

void drawSimpleLightModel(TGAImage& image, Model* model, Vec3f light_dir) {
	int width = image.get_width();
	int height = image.get_height();
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
			world_coords[j] = v;
		}
		// 重载^运算符实现外积, 从而计算得到三角面片的法线
		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		// 单位化法线
		n.normalize();
		// 法线与光照方向的内积(向量积)得到标量光照强度
		float intensity = n * light_dir;
		if (intensity > 0) {
			triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}
	}
}

int main(int argc, char** argv) {
	TGAImage image(1000, 1000, TGAImage::RGB);
	Model* model = new Model((PATH::RESOURCES + "african_head.obj").c_str());
	Vec3f light_dir(0, 0, -1);

	drawSimpleLightModel(image, model, light_dir);

	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}
