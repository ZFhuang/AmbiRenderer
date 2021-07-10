#include "AArect.hpp"

bool XY_Rect::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const
{
	// 计算光线与矩形的交点时间
	auto s = (k - r.origin().z()) / r.direction().z();
	if (s<s_min || s>s_max) {
		// 矩形在光线的判定框外
		return false;
	}
	// 计算光线与矩形的交点世界坐标
	auto x = r.origin().x() + s * r.direction().x();
	auto y = r.origin().y() + s * r.direction().y();
	if (x<x0 || x>x1 || y<y0 || y>y1) {
		// 光线与矩形无交点
		return false;
	}
	// 计算局部二维参数表面的交点坐标
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	// 计算局部交点时间
	rec.s = s;
	auto outward_normal = Vec3(0, 0, 1);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(s);
	return true;
}

bool XY_Rect::bounding_box(double time0, double time1, AABB& output_box) const
{
	// 用加减0.0001来生成小包围盒
	output_box = AABB(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
	return true;
}
