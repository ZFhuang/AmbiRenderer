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
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;
	Point3 center;
	double radius;
	shared_ptr<Material> mat_ptr;

private:
	static void get_sphere_uv(const Point3& p, double& u, double& v) {
		// 球坐标系公式, 从表面的法线转二维点, 直接修改引用来返回
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + pi;
		u = phi / (2 * pi);
		v = theta / pi;
	}
};
