#pragma once

#include "Ray.hpp"
#include "Material.hpp"
#include "AABB.hpp"
#include <memory>

using std::shared_ptr;

// 前置声明
class Material;

// 对当前射线已经发生的命中进行记录
struct HitRecord
{
	// 命中的点的坐标
	Point3 p;
	// 命中的点的法线
	Vec3 normal;
	// 命中时射线的长度比例
	double s;
	// 二维参数表面的坐标
	double u;
	double v;
	// 是否命中的是正面
	bool front_face;
	// 对命中对象材质的标志指针
	shared_ptr<Material> mat_ptr;

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
	// 返回可命中物体的包围盒, 这个最小包围盒是用来协助场景层次包围盒的
	// 有time参数得以处理动态模糊效果, 部分物体返回false代表无需包围盒
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const = 0;
};

class Translate :public Hittable {
public:
	Translate(shared_ptr<Hittable> p, const Vec3& displacement) :ptr(p), offset(displacement) {}
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
	shared_ptr<Hittable> ptr;
	Vec3 offset;
};

class RotateY :public Hittable {
public:
	RotateY(shared_ptr<Hittable> p, double angle);
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
	shared_ptr<Hittable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	AABB bbox;
};
