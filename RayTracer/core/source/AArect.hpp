#pragma once

#include "RTweekend.hpp"
#include "Hittable.hpp"

class XY_Rect : public Hittable {
public:
	XY_Rect() {}
	// 列表初始化顺序只与构造函数参数顺序有关
	XY_Rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> mat) :x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
	shared_ptr<Material> mp;
	double x0, x1, y0, y1, k;
};

class XZ_Rect : public Hittable {
public:
	XZ_Rect() {}
	// 列表初始化顺序只与构造函数参数顺序有关
	XZ_Rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<Material> mat) :x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
	shared_ptr<Material> mp;
	double x0, x1, z0, z1, k;
};

class YZ_Rect : public Hittable {
public:
	YZ_Rect() {}
	// 列表初始化顺序只与构造函数参数顺序有关
	YZ_Rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<Material> mat) :y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
	shared_ptr<Material> mp;
	double y0, y1, z0, z1, k;
};