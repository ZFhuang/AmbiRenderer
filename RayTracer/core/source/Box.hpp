#pragma once

#include "RTweekend.hpp"
#include "AArect.hpp"
#include "HittableList.hpp"

class Box: public Hittable{
public:
	Box(){}
	Box(const Point3& p0, const Point3& p1, shared_ptr<Material> ptr);
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
	// 只保存两个角点和6个面
	Point3 box_min;
	Point3 box_max;
	HittableList sides;
};