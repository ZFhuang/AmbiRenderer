#include <chrono>
#include <iostream>

#include "RayTracer.hpp"
#include "Sphere.hpp"
#include "Color.hpp"
#include "Material.hpp"

int run_RayTracer() {
	// 设置要绘制的长宽比和图像宽度
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 720;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	// 反走样的超采样次数
	const int sample_times = 400;
	// 光线的反弹次数
	const int max_depth = 100;

	// 设置需要用的几个对象材质指针, 参数是物体对材质的反照率
	auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
	auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	// 正常的折射效果
	auto material_left = make_shared<Dielectric>(1.5);
	// 折射指数小, 会导致折射变成反射
	auto material_right = make_shared<Dielectric>(0.5);

	// 设置场景对象
	HittableList scene;
	scene.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	scene.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
	// 这里左边放了两个玻璃球, 一个正面一个反面, 反面的玻璃球可以得到中空玻璃的效果
	scene.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	scene.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
	scene.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

	// 开始帧计时
	auto startTime = std::chrono::system_clock::now();

	// 设置相机
	Camera cam(aspect_ratio);

	// 测试输出, 保存在图片"test.ppm"中
	renderImage(image_width, image_height, cam, scene, sample_times, max_depth);

	// 结束帧计时, 输出渲染本帧所用的毫秒数
	auto endTime = std::chrono::system_clock::now();
	std::clog << "Frame time: "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
		<< "ms" << std::endl;
	return 0;
}

int renderImage(int image_width, int image_height, Camera cam, HittableList scene, int sampleTimes = 1, int max_depth = 2) {
	// 输出流重定向到图片文件
	freopen("result.ppm", "w", stdout);
	// 头信息
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// 注意图片是从上往下, 从左往右绘制的
	for (int j = image_height - 1; j >= 0; --j) {
		std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			// 计算这个像素射线计算应该着色的颜色
			Color pixel(0, 0, 0);
			// 进行超采样反走样, 也就是对每个像素都进行一点扰动然后采样多次来平均
			for (int k = 0; k < sampleTimes; ++k) {
				// 计算出当前像素映射到0-1之中的比例
				// 对每个像素索引随机扰动(0 - 1)从而让生成的射线在一个像素内有所变化, 达到超采样的效果
				double u = (static_cast<double>(i) + random_double()) / (image_width - 1);
				double v = (static_cast<double>(j) + random_double()) / (image_height - 1);
				// 利用这个比例计算对应到视平面的位置, 与相机原点相减就得到对应像素的视角射线向量
				Ray r = cam.getRay(u, v);
				// 累加这个像素着色的颜色
				pixel += ray_color(r, scene, max_depth);
			}

			// 写入颜色到图片的像素, 内部对像素值进行了裁剪
			write_color(std::cout, pixel, sampleTimes);
		}
	}
	std::clog << "\nDone." << std::endl;
	return 0;
}

Color ray_color(const Ray& r, const HittableList& scene, int depth) {
	if (depth <= 0) {
		// 如果直到递归层数极限也还没有命中背景(天空), 则理解为无法反射, 返回黑色
		return Color(0, 0, 0);
	}

	HitRecord rec;
	// 命中球的时候会随机进行反射, 最小距离设置为0.001, 防止精度误差导致的误反射
	if (scene.hit(r, 0.001, infinity, rec)) {
		// 暂存反射向量
		Ray scattered;
		// 暂存衰减值, 也就是能量被吸收的比例
		Color attenuation;
		// 调用物体表面的材质散射计算, 得到反射向量和亮度
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			// 进行递归, 每层返回都会衰减
			return attenuation * ray_color(scattered, scene, depth - 1);
		}
		// 没有材质的表面不需要颜色
		return Color(0, 0, 0);
	}

	// 没有命中的时候背景(光源)就被用来给全局着色了
	Vec3 unit_direction = unit_vector(r.direction());
	// 在白色和浅蓝色之间利用向量的y方向进行加权平均, 由于向量被归一化了
	// 因此y的变化并不是均匀的, 而是会表现出弧线的性质. 权重从0.5到1
	auto s = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - s) * Color(1.0, 1.0, 1.0) + s * Color(0.5, 0.7, 1.0);
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
