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

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
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

void drawWireFrame(Model* model, TGAImage& image) {
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
			line(x0, y0, x1, y1, image, white);
		}
	}
}

int main(int argc, char** argv) {
	TGAImage image(1000, 1000, TGAImage::RGB);
	Model* model = new Model((PATH::RESOURCES + "african_head.obj").c_str());

	drawWireFrame(model, image);

	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}
