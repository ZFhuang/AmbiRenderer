#include "RayTracer.h"
using namespace std;

int run_RayTracer() {
	// 设置要绘制的画面大小
	const int image_width = 256;
	const int image_height = 256;

	// 将输出重定向到图片文件上
	FILE* outFile;
	freopen_s(&outFile, "result.ppm", "w", stdout);
	// 写入头信息
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// 用双重循环来写入图片信息, 这里要注意图片是从上往下, 从左往右绘制的, 但是平时我们习惯是从下网上
	for (int j = image_height - 1; j >= 0; --j) {
		for (int i = 0; i < image_width; ++i) {
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.25f;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	return 0;
}
