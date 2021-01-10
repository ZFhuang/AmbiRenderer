#pragma once

#include "Vec3.hpp"

class Ray
{
public:
	Ray() { ; }
	Ray(const Point3& origin, const Vec3& direction) : ori(origin), dir(direction) {}

	// 返回射线起点
	Point3 origin() const { return ori; }
	// 返回射线方向向量
	Vec3 direction() const { return dir; }
	// 返回射线真正向量, 从起点开始, 方向是dir, 由输入参数s决定射线的正反
	Point3 at(double s) const { return ori + dir * s; }

private:
	Point3 ori;
	Vec3 dir;
};

// 根据输入的射线返回着色
inline Color ray_color(const Ray& r) {
	// 先归一化当前射线向量
	Vec3 unit_direction = unit_vector(r.direction());
	// 在白色和浅蓝色之间利用向量的y方向进行加权平均
	// 权重从0.5到1
	double s = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - s) * Color(1.0, 1.0, 1.0) + s * Color(0.5, 0.7, 1.0);
}
