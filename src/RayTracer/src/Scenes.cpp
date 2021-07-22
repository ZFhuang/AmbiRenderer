#include "Scenes.hpp"
#include "Sphere.hpp"
#include "MovingSphere.hpp"
#include "AArect.hpp"
#include "Box.hpp"
#include "ConstantMedium.hpp"
#include "BVH.hpp"
#include "../../Config.hpp"

TestScene::TestScene()
{
	// 设置需要用的几个对象材质指针
	auto material_ground = make_shared<Metal>(Color(0.7, 0.7, 0.7), 0.3);
	auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	// 正常的折射效果
	auto material_left = make_shared<Dielectric>(1.5);
	// 金属球
	auto material_right = make_shared<Metal>(Color(0.9, 0.9, 0.9), 0);

	_hitlist.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	_hitlist.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
	// 这里左边放了两个玻璃球, 一个正面一个反面, 反面的玻璃球可以得到中空玻璃的效果
	_hitlist.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	_hitlist.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
	_hitlist.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));
}

const std::string TestScene::get_name() const
{
	return "Test Scene";
}

HittableList TestScene::get_HittableList() const
{
	return _hitlist;
}

RandomScene::RandomScene()
{
	// 地面使用棋盘格实体纹理
	auto ground_mat = make_shared<CheckerTexture>(Color(0.5, 0.5, 0.5), Color(0.9, 0.9, 0.9));
	_hitlist.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(ground_mat)));

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
					_hitlist.add(make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphere_mat));
				}
				else if (choose_mat < 0.9) {
					// 少部分反射metal
					auto albedo = Color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_mat = make_shared<Metal>(albedo, fuzz);
					_hitlist.add(make_shared<Sphere>(center, 0.2, sphere_mat));
				}
				else {
					// 更少部分折射dielect
					sphere_mat = make_shared<Dielectric>(1.5);
					_hitlist.add(make_shared<Sphere>(center, 0.2, sphere_mat));
				}
			}
		}
	}

	auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	auto material_right = make_shared<Dielectric>(1.5);
	auto material_left = make_shared<Metal>(Color(0.9, 0.9, 0.9), 0);

	_hitlist.add(make_shared<Sphere>(Point3(0.0, 1.0, 0), 1, material_center));
	_hitlist.add(make_shared<Sphere>(Point3(4.0, 1.0, 0), 1, material_right));
	_hitlist.add(make_shared<Sphere>(Point3(4.0, 1.0, 0), -0.9, material_right));
	_hitlist.add(make_shared<Sphere>(Point3(-4.0, 1.0, 0), 1, material_left));
}

const std::string RandomScene::get_name() const
{
	return "Random Scene";
}

HittableList RandomScene::get_HittableList() const
{
	return _hitlist;
}

PerlinScene::PerlinScene()
{
	auto perlin_texture = make_shared<PerlinNoiseTexture>(4);
	auto ground_mat = make_shared<CheckerTexture>(Color(0.5, 0.5, 0.5), Color(0.9, 0.9, 0.9));
	// 这里Perlin噪声作为漫反射纹理应用
	_hitlist.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(ground_mat)));
	_hitlist.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlin_texture)));
}

const std::string PerlinScene::get_name() const
{
	return "Perlin Scene";
}

HittableList PerlinScene::get_HittableList() const
{
	return _hitlist;
}

EarthScene::EarthScene()
{
	// 暂时使用了绝对路径
	auto earth_texture = make_shared<ImageTexture>(PATH::RESOURCES+"earthmap.jpg");
	_hitlist.add(make_shared<Sphere>(Point3(0, 0, 0), 2, make_shared<Lambertian>(earth_texture)));
}

const std::string EarthScene::get_name() const
{
	return "Earth Scene";
}

HittableList EarthScene::get_HittableList() const
{
	return _hitlist;
}

SimpleLightScene::SimpleLightScene()
{
	auto perlinTex = make_shared<PerlinNoiseTexture>(4);
	// 场景两个球
	_hitlist.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlinTex)));
	_hitlist.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlinTex)));

	// 光源材质传递给矩形, k是轴向距离, 注意光源设置的值比较大(比1大)为了让其经受住更多次反弹的削减
	_hitlist.add(make_shared<XY_Rect>(3, 5, 1, 3, -2, make_shared<DiffuseLight>(Color(4, 4, 4))));
}

const std::string SimpleLightScene::get_name() const
{
	return "Simple Light Scene";
}

HittableList SimpleLightScene::get_HittableList() const
{
	return _hitlist;
}

CornelOriScene::CornelOriScene()
{
	auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto light = make_shared<DiffuseLight>(Color(15, 15, 15));
	// 构造墙壁
	_hitlist.add(make_shared<YZ_Rect>(0, 555, 0, 555, 555, green));
	_hitlist.add(make_shared<YZ_Rect>(0, 555, 0, 555, 0, red));
	_hitlist.add(make_shared<XZ_Rect>(213, 343, 227, 332, 554, light));
	_hitlist.add(make_shared<XZ_Rect>(0, 555, 0, 555, 0, white));
	_hitlist.add(make_shared<XZ_Rect>(0, 555, 0, 555, 555, white));
	_hitlist.add(make_shared<XY_Rect>(0, 555, 0, 555, 555, white));
	// 初始化两个盒子
	shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	// 改变位置并放入
	box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
	_hitlist.add(box1);
	box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
	_hitlist.add(box2);
}

const std::string CornelOriScene::get_name() const
{
	return "Connel Original Scene";
}

HittableList CornelOriScene::get_HittableList() const
{
	return _hitlist;
}

CornelSmokeScene::CornelSmokeScene()
{
	HittableList _hitlist;
	auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
	// 构造墙壁
	_hitlist.add(make_shared<YZ_Rect>(0, 555, 0, 555, 555, green));
	_hitlist.add(make_shared<YZ_Rect>(0, 555, 0, 555, 0, red));
	_hitlist.add(make_shared<XZ_Rect>(113, 443, 127, 432, 554, light));
	_hitlist.add(make_shared<XZ_Rect>(0, 555, 0, 555, 0, white));
	_hitlist.add(make_shared<XZ_Rect>(0, 555, 0, 555, 555, white));
	_hitlist.add(make_shared<XY_Rect>(0, 555, 0, 555, 555, white));
	// 初始化两个盒子
	shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	// 改变位置并放入
	box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
	// 将盒子转为雾属性
	_hitlist.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
	box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
	_hitlist.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));
}

const std::string CornelSmokeScene::get_name() const
{
	return "Connel Smoke Scene";
}

HittableList CornelSmokeScene::get_HittableList() const
{
	return _hitlist;
}

FinalScene::FinalScene()
{
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
	_hitlist.add(make_shared<BVH_Node>(boxes1, 0, 1));

	// 布置光源
	auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
	_hitlist.add(make_shared<XZ_Rect>(123, 423, 147, 412, 554, light));

	// 长曝光的移动球
	auto center1 = Point3(400, 400, 200);
	auto center2 = center1 + Vec3(30, 0, 0);
	auto moving_sphere_mat = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
	_hitlist.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_mat));

	// 玻璃球
	_hitlist.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));

	// 金属球
	_hitlist.add(make_shared<Sphere>(Point3(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

	// 有雾的玻璃球
	auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
	_hitlist.add(boundary);
	_hitlist.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
	// 覆盖场景的雾
	boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
	_hitlist.add(make_shared<ConstantMedium>(boundary, 0.0001, Color(1, 1, 1)));

	// 地球
	auto earth_mat = make_shared<Lambertian>(make_shared<ImageTexture>(PATH::RESOURCES + "earthmap.jpg"));
	_hitlist.add(make_shared<Sphere>(Point3(400, 200, 400), 100, earth_mat));

	// 噪声球
	auto perlin_mat = make_shared<PerlinNoiseTexture>(0.1);
	_hitlist.add(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<Lambertian>(perlin_mat)));

	// 随机白球填充一个盒子区域
	HittableList boxes2;
	auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<Sphere>(Point3::random(0, 165), 10, white));
	}
	_hitlist.add(make_shared<Translate>(make_shared<RotateY>(make_shared<BVH_Node>(boxes2, 0.0, 1.0), 15), Vec3(-100, 270, 395)));
}

const std::string FinalScene::get_name() const
{
	return "Final Scene";
}

HittableList FinalScene::get_HittableList() const
{
	return _hitlist;
}
