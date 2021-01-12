#pragma once

#include "Vec3.hpp"

class Ray
{
public:
	Ray() {}
	Ray(const Point3& origin, const Vec3& direction) : ori(origin), dir(direction) {}

	// 返回射线起点
	Point3 origin() const { return ori; }
	// 返回射线方向向量
	Vec3 direction() const { return dir; }
	// 返回射线真正向量, 从起点开始, 方向是dir, 由输入参数s决定射线的正反
	Point3 at(double s) const { return ori + dir * s; }

private:
	// 射线起点
	Point3 ori;
	// 射线方向向量
	Vec3 dir;
};
