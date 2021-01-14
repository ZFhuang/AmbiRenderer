#pragma once

#include "Vec3.hpp"
#include "Hittable.hpp"

// 继承了Hittable的球类, 用来封装绘制一个球相关的东西, 核心是球心和半径属性
class Sphere : public Hittable
{
public:
	Sphere() {};
	// 球心, 半径, 材质指针
	Sphere(Point3 cen, double r, shared_ptr<Material> m) :center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	Point3 center;
	double radius;
	shared_ptr<Material> mat_ptr;
};
