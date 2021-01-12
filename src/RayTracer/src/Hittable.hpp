#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

// 对当前射线已经发生的命中进行记录
struct HitRecord
{
	// 命中的点的坐标
	Point3 p;
	// 命中的点的法线
	Vec3 normal;
	// 命中时射线的长度比例
	double s;
	// 是否命中的是正面
	bool front_face;

	// 通过输入的法线和当前朝外的法线来判断是否从正面命中
	inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
		// 当射线方向和向外的法线不同向(<0)时, 代表命中正面
		front_face = dot(r.direction(), outward_normal) < 0;
		// 利用这个命中的方向来写入正确(指向表面外)的点法线到记录中
		normal = front_face ? outward_normal : -outward_normal;
	}
};

// 一个用于继承作为可命中对象的抽象类, 通过重写hit函数来判断是否命中
class Hittable
{
public:
	// 用于重写的纯虚函数hit, 判断是否命中当前对象, 返回的是bool, 结果在rec中
	// 这里判断命中用到的是min, max区间, 这能让我们对后来的代码进行进一步优化
	// 传递Hit_record用来记录发生的命中
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const = 0;
};
