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

// 用函数模拟射线射中某个球, 圆心center, 半径radius, 射线r判断是否命中
bool hit_sphere(const Point3& center, double radius, const Ray& r) {
	// 核心是求出线代入球方程得到的根的数量, 无解(d<0)则没有相交, 这里只处理不为0的正数
	// 先纸面上假设射线交点, 联立球面方程和射线方程得到一个二次方程如P17
	// 按照方程求解, 只需要求判别式即可
	Vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;
	// 二次方程判别式
	auto discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

// 根据输入的射线返回着色
inline Color ray_color(const Ray& r) {
	if (hit_sphere(Point3(0, 0, 1), 0.5, r)) {
		// 如果当前射线命中一个位于0,0,1, 半径0.5的球, 则返回灰色作为颜色
		return Color(0.7, 0.7, 0.7);
	}

	// 先归一化当前射线向量
	Vec3 unit_direction = unit_vector(r.direction());
	// 在白色和浅蓝色之间利用向量的y方向进行加权平均, 由于向量被归一化了
	// 因此y的变化并不是均匀的, 而是会表现出弧线的性质. 权重从0.5到1
	double s = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - s) * Color(1.0, 1.0, 1.0) + s * Color(0.5, 0.7, 1.0);
}
