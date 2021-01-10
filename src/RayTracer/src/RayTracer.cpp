#include "RayTracer.hpp"
#include "Vec3.hpp"
#include "Color.hpp"

int run_RayTracer() {
	// 这里设置要绘制的图像大小
	const int image_width = 256;
	const int image_height = 256;

	Vec3 a(1, 2, 3);

	// 测试输出, 保存在图片"test.ppm"中
	return renderImage(image_width, image_height);;
}

int renderImage(int image_width, int image_height) {
	// 输出流重定向到图片文件
	freopen("test.ppm", "w", stdout);
	// 头信息
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// 注意图片是从上往下, 从左往右绘制的
	for (int j = image_height - 1; j >= 0; --j) {
		std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			// 像素内容暂时是测试用的渐变颜色, 构造像素对象
			Color pixel = Color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25f);
			// 写入颜色到图片的像素
			write_color(std::cout, pixel);
		}
	}
	std::clog << "\nDone." << std::endl;
	return 0;
}

int testImage(int image_width, int image_height) {
	// 将输出流重定向到图片文件上方便下面的写入
	freopen("test.ppm", "w", stdout);
	// 写入头信息
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// 用双重循环来写入图片信息, 要注意图片是从上往下, 从左往右绘制的, 平时我们习惯是从下往上
	for (int j = image_height - 1; j >= 0; --j) {
		// 由于标准输出流被文件占用, 因此将当前执行进度输出到日志流, 一样会显示在命令行中
		// 用\r来让输出保持在同一行
		std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			// 写入的内容暂时是测试用的渐变颜色
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.25f;
			// 由于ppm只支持整数因此需要进行类型转换
			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);
			// 写入
			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	std::clog << "\nDone." << std::endl;
	return 0;
}
