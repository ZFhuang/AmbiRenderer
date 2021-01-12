#include <iostream>
#include <memory>

#include "RayTracer.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "RTweekend.hpp"

using std::shared_ptr;
using std::make_shared;

Color ray_color(const Ray& r, const HittableList& scene) {
	HitRecord rec;
	// 命中球的时候
	if (scene.hit(r, 0, infinity, rec)) {
		// 将法线坐标作为颜色, 归一化为正数(0-1)
		return 0.5 * (rec.normal + Color(1, 1, 1));
	}

	// 没有命中的时候绘制背景, 先归一化当前射线向量
	Vec3 unit_direction = unit_vector(r.direction());
	// 在白色和浅蓝色之间利用向量的y方向进行加权平均, 由于向量被归一化了
	// 因此y的变化并不是均匀的, 而是会表现出弧线的性质. 权重从0.5到1
	auto s = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - s) * Color(1.0, 1.0, 1.0) + s * Color(0.5, 0.7, 1.0);
}

int run_RayTracer() {
	// 设置要绘制的图像宽度和长宽比
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 720;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	// 设置场景对象
	HittableList scene;
	scene.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	scene.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	// 设置渲染的相机参数, 世界坐标系中的视平面大小和焦长
	double viewport_height = 2.0;
	double viewport_width = aspect_ratio * viewport_height;
	double focal_length = 1.0;
	// 处理后的相机参数
	auto camera_args = setCamera(viewport_width, viewport_height, focal_length);

	// 测试输出, 保存在图片"test.ppm"中
	return renderImage(image_width, image_height, camera_args, scene);
}

int renderImage(int image_width, int image_height, Vec3* camera_args, HittableList scene) {
	// 输出流重定向到图片文件
	freopen("result.ppm", "w", stdout);
	// 头信息
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// 注意图片是从上往下, 从左往右绘制的
	for (int j = image_height - 1; j >= 0; --j) {
		std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			// 计算出当前像素映射到0-1之中的比例
			double u = static_cast<double>(i) / (image_width - 1);
			double v = static_cast<double>(j) / (image_height - 1);
			// 利用这个比例计算对应到视平面的位置, 与相机原点相减就得到对应像素的视角射线向量
			Ray r(camera_args[0], camera_args[3] + u * camera_args[1] + v * camera_args[2] - camera_args[0]);
			// 计算这个射线计算应该着色的颜色
			Color pixel = ray_color(r, scene);
			// 写入颜色到图片的像素
			write_color(std::cout, pixel);
		}
	}
	std::clog << "\nDone." << std::endl;
	return 0;
}

Vec3* setCamera(double viewport_width, double viewport_height, double focal_length) {
	// 相机当前位置
	Point3 origin = Point3(0, 0, 0);
	// 水平方向的长度向量
	Vec3 horizontal = Vec3(viewport_width, 0, 0);
	// 垂直方向的长度向量
	Vec3 vertical = Vec3(0, viewport_height, 0);
	// 计算出视平面左下角的真实位置
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
	// 填充到参数集中
	Vec3* camera_args=new Vec3[4];
	camera_args[0] = origin;
	camera_args[1] = horizontal;
	camera_args[2] = vertical;
	camera_args[3] = lower_left_corner;
	return camera_args;
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
