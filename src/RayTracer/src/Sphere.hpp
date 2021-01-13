#pragma once

#include "Vec3.hpp"
#include "Hittable.hpp"

// 继承了Hittable的球类, 用来封装绘制一个球相关的东西, 核心是球心和半径属性
class Sphere : public Hittable
{
public:
	Sphere() {};
	Sphere(Point3 cen, double r) :center(cen), radius(r) {};
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	Point3 center;
	double radius;
};
