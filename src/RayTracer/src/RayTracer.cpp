#include <chrono>
#include <iostream>

#include "RayTracer.hpp"
#include "Sphere.hpp"
#include "MovingSphere.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include "BVH.hpp"
#include "Texture.hpp"
#include "AArect.hpp"
#include "Box.hpp"
#include "ConstantMedium.hpp"

int run_RayTracer() {
	// 设置场景对象
	HittableList scene;
	Point3 lookfrom;
	Point3 lookat;
	// 设置相机垂直fov
	double vfov = 20.0;
	// 光圈大小
	double aperture = 0;
	// 背景色
	Color background(0, 0, 0);
	// 图像宽高
	double aspect_ratio = 16.0 / 9.0;
	// 设置图像大小
	int image_width = 1500;
	// 反走样的超采样次数
	int sample_times = 1000;
	// 光线的反弹次数
	int max_depth = 50;

	int scene_choose = 8;
	switch (scene_choose)
	{
	case 1:
		// 应用BVH加速, 用当前场景对象初始化BVH树
		scene = HittableList(make_shared<BVH_Node>(random_scene(), 0, 1));
		// 设置相机参数
		background = Color(0.7, 0.8, 1);
		lookfrom = Point3(13, 2, 3);
		lookat = Point3(0, 0, 0);
		break;
	case 2:
		scene = HittableList(make_shared<BVH_Node>(test_scene(), 0, 1));
		background = Color(0.7, 0.8, 1);
		lookfrom = Point3(13, 2, 3);
		lookat = Point3(0, 0, 0);
		break;
	case 3:
		scene = HittableList(make_shared<BVH_Node>(two_perlin_scene(), 0, 1));
		background = Color(0.7, 0.8, 1);
		lookfrom = Point3(13, 2, 3);
		lookat = Point3(0, 0, 0);
		break;
	case 4:
		scene = HittableList(make_shared<BVH_Node>(earth_scene(), 0, 1));
		background = Color(0.7, 0.8, 1);
		lookfrom = Point3(13, 2, 3);
		lookat = Point3(0, 0, 0);
		break;
	case 5:
		scene = HittableList(make_shared<BVH_Node>(simple_light_scene(), 0, 1));
		background = Color(0, 0, 0);
		lookfrom = Point3(26, 3, 6);
		lookat = Point3(0, 2, 0);
		break;
	case 6:
		scene = HittableList(make_shared<BVH_Node>(cornell_box_scene(), 0, 1));
		aspect_ratio = 1, 0;
		background = Color(0, 0, 0);
		lookfrom = Point3(278, 278, -800);
		lookat = Point3(278, 278, 0);
		vfov = 40.0;
		break;
	case 7:
		scene = HittableList(make_shared<BVH_Node>(cornell_smoke_scene(), 0, 1));
		aspect_ratio = 1, 0;
		background = Color(0, 0, 0);
		lookfrom = Point3(278, 278, -800);
		lookat = Point3(278, 278, 0);
		vfov = 40.0;
		break;
	case 8:
		scene = HittableList(make_shared<BVH_Node>(final_scene(), 0, 1));
		aspect_ratio = 1, 0;
		background = Color(0, 0, 0);
		lookfrom = Point3(478, 278, -600);
		lookat = Point3(278, 278, 0);
		vfov = 40.0;
		break;
	default:
		background = Color(1, 1, 1);
		lookfrom = Point3(26, 3, 6);
		lookat = Point3(0, 2, 0);
		break;
	}

	// 计算图像尺寸
	int image_height = static_cast<int>(image_width / aspect_ratio);

	// 开始帧计时
	auto startTime = std::chrono::system_clock::now();

	Vec3 vup(0, 1, 0);
	// 计算想要对焦的物体的距离
	auto dist_to_focus = 10.0;

	// 为了计算动态模糊, 相机也要在最后加上快门的开闭时间
	Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, dist_to_focus, aperture, 0.0, 1.0);

	// 测试输出, 保存在图片"test.ppm"中
	renderImage(image_width, image_height, cam, scene, background, sample_times, max_depth);

	// 结束帧计时, 输出渲染本帧所用的毫秒数
	auto endTime = std::chrono::system_clock::now();
	std::clog << "Frame time: "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
		<< "ms" << std::endl;
	return 0;
}

int renderImage(int image_width, int image_height, Camera cam, HittableList scene, Color background, int sampleTimes = 1, int max_depth = 2) {
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
				pixel += ray_color(r, scene, background, max_depth);
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

Color ray_color(const Ray& r, const HittableList& scene, const Color& background, int depth) {
	if (depth <= 0) {
		// 太远距离时返回无
		return Color(0, 0, 0);
	}

	HitRecord rec;
	// 命中球的时候会随机进行反射, 最小距离设置为0.001, 防止精度误差导致的误反射
	// 太远而没有命中返回无
	if (!scene.hit(r, 0.001, infinity, rec)) {
		return background;
	}

	// 暂存反射向量
	Ray scattered;
	// 暂存衰减值, 也就是能量被吸收的比例
	Color attenuation;
	Color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
	// 遇到发光材质时不继续递归而是返回光源颜色
	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
		// 进行递归, 每层返回都会衰减
		return emitted;
	}

	// 材质发光值+衰减值*递归值
	return emitted + attenuation * ray_color(scattered, scene, background, depth - 1);
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

	// 地面使用棋盘格实体纹理
	auto ground_mat = make_shared<CheckerTexture>(Color(0.5, 0.5, 0.5), Color(0.9, 0.9, 0.9));
	scene.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(ground_mat)));

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
	//scene.add(make_shared<Sphere>(Point3(4.0, 1.0, 0), -0.9, material_right));
	scene.add(make_shared<Sphere>(Point3(-4.0, 1.0, 0), 1, material_left));

	return scene;
}

HittableList two_perlin_scene()
{
	HittableList scene;

	auto perlin_texture = make_shared<PerlinNoiseTexture>(4);
	auto ground_mat = make_shared<CheckerTexture>(Color(0.5, 0.5, 0.5), Color(0.9, 0.9, 0.9));
	// 这里Perlin噪声作为漫反射纹理应用
	scene.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(ground_mat)));
	scene.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlin_texture)));
	return scene;
}

HittableList earth_scene()
{
	HittableList scene;
	// 暂时使用了绝对路径
	auto earth_texture = make_shared<ImageTexture>("C:/Work/AmbiRenderer/src/Resources/earthmap.jpg");
	scene.add(make_shared<Sphere>(Point3(0, 0, 0), 2, make_shared<Lambertian>(earth_texture)));
	return scene;
}

HittableList simple_light_scene()
{
	HittableList scene;
	auto perlinTex = make_shared<PerlinNoiseTexture>(4);
	// 场景两个球
	scene.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlinTex)));
	scene.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlinTex)));

	// 光源材质传递给矩形, k是轴向距离, 注意光源设置的值比较大(比1大)为了让其经受住更多次反弹的削减
	scene.add(make_shared<XY_Rect>(3, 5, 1, 3, -2, make_shared<DiffuseLight>(Color(4, 4, 4))));
	return scene;
}

HittableList cornell_box_scene()
{
	HittableList scene;
	auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto light = make_shared<DiffuseLight>(Color(15, 15, 15));
	// 构造墙壁
	scene.add(make_shared<YZ_Rect>(0, 555, 0, 555, 555, green));
	scene.add(make_shared<YZ_Rect>(0, 555, 0, 555, 0, red));
	scene.add(make_shared<XZ_Rect>(213, 343, 227, 332, 554, light));
	scene.add(make_shared<XZ_Rect>(0, 555, 0, 555, 0, white));
	scene.add(make_shared<XZ_Rect>(0, 555, 0, 555, 555, white));
	scene.add(make_shared<XY_Rect>(0, 555, 0, 555, 555, white));
	// 初始化两个盒子
	shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	// 改变位置并放入
	box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
	scene.add(box1);
	box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
	scene.add(box2);
	return scene;
}

HittableList cornell_smoke_scene()
{
	HittableList scene;
	auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
	// 构造墙壁
	scene.add(make_shared<YZ_Rect>(0, 555, 0, 555, 555, green));
	scene.add(make_shared<YZ_Rect>(0, 555, 0, 555, 0, red));
	scene.add(make_shared<XZ_Rect>(113, 443, 127, 432, 554, light));
	scene.add(make_shared<XZ_Rect>(0, 555, 0, 555, 0, white));
	scene.add(make_shared<XZ_Rect>(0, 555, 0, 555, 555, white));
	scene.add(make_shared<XY_Rect>(0, 555, 0, 555, 555, white));
	// 初始化两个盒子
	shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	// 改变位置并放入
	box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
	// 将盒子转为雾属性
	scene.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
	box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
	scene.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));
	return scene;
}

HittableList final_scene()
{
	HittableList scene;
	auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

	// 生成y随机波动的盒子作为地板
	HittableList boxes1;
	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			// 每个盒子100宽度
			double w = 100.0;
			// xy是均匀分布的
			double x0 = -1000.0 + i * w;
			double z0 = -1000.0 + j * w;
			double y0 = 0.0;
			double x1 = x0 + w;
			// 随机高度
			double y1 = random_double(1, 101);
			double z1 = z0 + w;
			boxes1.add(make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
		}
	}
	scene.add(make_shared<BVH_Node>(boxes1, 0, 1));

	// 布置光源
	auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
	scene.add(make_shared<XZ_Rect>(123,423,147,412,554,light));

	// 长曝光的移动球
	auto center1 = Point3(400, 400, 200);
	auto center2 = center1 + Vec3(30, 0, 0);
	auto moving_sphere_mat = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
	scene.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_mat));

	// 玻璃球
	scene.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));

	// 金属球
	scene.add(make_shared<Sphere>(Point3(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

	// 有雾的玻璃球
	auto boundary = make_shared<Sphere>(Point3(360,150,145), 70, make_shared<Dielectric>(1.5));
	scene.add(boundary);
	scene.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
	// 覆盖场景的雾
	boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
	scene.add(make_shared<ConstantMedium>(boundary, 0.0001, Color(1,1,1)));

	// 地球
	auto earth_mat = make_shared<Lambertian>(make_shared<ImageTexture>("C:/Work/AmbiRenderer/src/Resources/earthmap.jpg"));
	scene.add(make_shared<Sphere>(Point3(400, 200, 400), 100, earth_mat));

	// 噪声球
	auto perlin_mat = make_shared<PerlinNoiseTexture>(0.1);
	scene.add(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<Lambertian>(perlin_mat)));

	// 随机白球填充一个盒子区域
	HittableList boxes2;
	auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<Sphere>(Point3::random(0, 165), 10, white));
	}
	scene.add(make_shared<Translate>(make_shared<RotateY>(make_shared<BVH_Node>(boxes2, 0.0, 1.0), 15), Vec3(-100, 270, 395)));

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
