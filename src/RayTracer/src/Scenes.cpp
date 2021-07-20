#include "Scenes.hpp"
#include "Sphere.hpp"


TestScene::TestScene()
{
	HittableList scene;

	// 设置需要用的几个对象材质指针
	auto material_ground = make_shared<Metal>(Color(0.7, 0.7, 0.7), 0.3);
	auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	// 正常的折射效果
	auto material_left = make_shared<Dielectric>(1.5);
	// 金属球
	auto material_right = make_shared<Metal>(Color(0.9, 0.9, 0.9), 0);

	scene.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	scene.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
	// 这里左边放了两个玻璃球, 一个正面一个反面, 反面的玻璃球可以得到中空玻璃的效果
	scene.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	scene.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
	scene.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));
	_hitlist = scene;
}

const std::string TestScene::get_name() const
{
	return "Test Scene";
}

HittableList TestScene::get_HittableList() const
{
	return _hitlist;
}
