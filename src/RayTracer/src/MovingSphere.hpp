#pragma once

#include "RTweekend.hpp"
#include "Hittable.hpp"

class MovingSphere :public Hittable
{
public:
	MovingSphere() {}
	MovingSphere(Point3 cen0, Point3 cen1, double _time0, double _time1, double r, shared_ptr<Material> mat)
		:center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(mat) {}

	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	Point3 center(double t) const;

	Point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<Material> mat_ptr;
};
