#include <chrono>
#include <iostream>

#include "RayTracer.hpp"
#include "Sphere.hpp"
#include "MovingSphere.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include "BVH.hpp"

int run_RayTracer() {
	// 设置图像宽度和宽高比
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 560;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	// 反走样的超采样次数
	const int sample_times = 100;
	// 光线的反弹次数
	const int max_depth = 50;

	// 设置场景对象
	HittableList scene = test_scene();

	// 开始帧计时
	auto startTime = std::chrono::system_clock::now();

	// 设置相机参数
	Point3 lookfrom(13, 2, 3);
	Point3 lookat(0, 0, 0);
	Vec3 vup(0, 1, 0);
	// 设置相机垂直fov
	const double vfov = 20;
	// 计算想要对焦的物体的距离
	auto dist_to_focus = 10.0;
	// 光圈大小
	double aperture = 0.1;
	// 为了计算动态模糊, 相机也要在最后加上快门的开闭时间
	Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

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
		std::clog << "\rProcessing: " << static_cast<double>(image_height - j) / image_height * 100 << "%;  ";
		auto line_startTime = std::chrono::system_clock::now();
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
		// 结束行计时, 输出剩余时间
		auto line_endTime = std::chrono::system_clock::now();
		std::clog << "Scantime remaining: "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(line_endTime - line_startTime).count() / 60000.0 * j
			<< " min                  " << std::flush;
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

HittableList test_scene() {
	// 设置需要用的几个对象材质指针
	auto material_ground = make_shared<Metal>(Color(0.7, 0.7, 0.7), 0.3);
	auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	// 正常的折射效果
	auto material_left = make_shared<Dielectric>(1.5);
	// 金属球
	auto material_right = make_shared<Metal>(Color(0.9, 0.9, 0.9), 0);

	HittableList scene;
	scene.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	scene.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
	// 这里左边放了两个玻璃球, 一个正面一个反面, 反面的玻璃球可以得到中空玻璃的效果
	scene.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	scene.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
	scene.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));
	return scene;
}

HittableList random_scene() {
	HittableList scene;

	auto ground_mat = make_shared<Metal>(Color(0.5, 0.5, 0.5), 0.3);
	scene.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_mat));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<Material> sphere_mat;

				if (choose_mat < 0.7) {
					// 大部分是漫反射lambertian
					auto albedo = Color::random() * Color::random();
					sphere_mat = make_shared<Lambertian>(albedo);
					// 在场景里给漫反射物体加上运动来渲染动态模糊
					// 计算目标位置
					auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
					// 加入运动的持续时间
					scene.add(make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphere_mat));
				}
				else if (choose_mat < 0.9) {
					// 少部分反射metal
					auto albedo = Color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_mat = make_shared<Metal>(albedo, fuzz);
					scene.add(make_shared<Sphere>(center, 0.2, sphere_mat));
				}
				else {
					// 更少部分折射dielect
					sphere_mat = make_shared<Dielectric>(1.5);
					scene.add(make_shared<Sphere>(center, 0.2, sphere_mat));
				}
			}
		}
	}

	auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	auto material_right = make_shared<Dielectric>(1.5);
	auto material_left = make_shared<Metal>(Color(0.9, 0.9, 0.9), 0);

	scene.add(make_shared<Sphere>(Point3(0.0, 1.0, 0), 1, material_center));
	scene.add(make_shared<Sphere>(Point3(4.0, 1.0, 0), 1, material_right));
	scene.add(make_shared<Sphere>(Point3(4.0, 1.0, 0), -0.9, material_right));
	scene.add(make_shared<Sphere>(Point3(-4.0, 1.0, 0), 1, material_left));

	return scene;
}

HittableList bvh_scene()
{
	// 设置需要用的几个对象材质指针
	auto material_ground = make_shared<Metal>(Color(0.7, 0.7, 0.7), 0.3);
	auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	// 正常的折射效果
	auto material_left = make_shared<Dielectric>(1.5);
	// 金属球
	auto material_right = make_shared<Metal>(Color(0.9, 0.9, 0.9), 0);

	HittableList objs;
	objs.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	objs.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
	// 这里左边放了两个玻璃球, 一个正面一个反面, 反面的玻璃球可以得到中空玻璃的效果
	objs.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	objs.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
	objs.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));
	HittableList scene;
	scene.add(make_shared<BVH_Node>(objs, 0, 1));
	return scene;
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
