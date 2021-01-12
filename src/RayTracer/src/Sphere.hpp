﻿#pragma once

#include "Vec3.hpp"
#include "Hittable.hpp"

// 继承了Hittable的球类, 用来封装绘制一个球相关的东西, 核心是球心和半径属性
class Sphere : public Hittable
{
public:
	Sphere() {};
	Sphere(Point3 cen, double r) :center(cen), radius(r) {};
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	Point3 center;
	double radius;
};

// 继承的虚函数进行重写, 同样是使用射线代入球方程的思路
inline bool Sphere::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const {
	// 核心是求出线代入球方程得到的根的数量, 无解(d<0)则没有相交, 这里只处理不为0的正数
	// 先纸面上假设射线交点, 联立球面方程和射线方程得到一个二次方程如P17, 按照方程求解, 只需要求判别式即可
	Vec3 oc = r.origin() - center;
	// a: 向量自己的点乘可以改写为向量长度的平方, 减少运算量
	auto a = r.direction().length_squa();
	// b: 如果使用b/2的话可以将式子约简掉几次数乘
	auto half_b = dot(oc, r.direction());
	// c: 同样用长度平方来减少点乘的使用
	auto c = oc.length_squa() - radius * radius;
	// 二次方程判别式
	auto discriminant = half_b * half_b - a * c;
	// 根据判别式, 返回射线交点
	if (discriminant < 0) {
		// 没有命中
		return false;
	}
	auto sqrtd = sqrt(discriminant);

	// 这里返回方程较小的根. 这个根是对应射线的长度参数
	auto root = (-half_b - sqrtd) / a;
	// 先判断小根是否在射线步长命中区域内
	if (root<s_min || root>s_max) {
		// 不在区域内则检测较大的根
		root = (-half_b + sqrtd) / a;
		if (root < s_min || root > s_max) {
			// 都不命中则返回不命中
			return false;
		}
	}

	// 存在命中则将命中结果写入rec中, 因为rec是传引用因此修改有效
	// 命中时的射线长度比例
	rec.s = root;
	// 命中的点的位置
	rec.p = r.at(rec.s);
	// 命中处的朝外法线值
	Vec3 out_normal = (rec.p - center) / radius;
	// 自动设定正确法线和命中内外的属性
	rec.set_face_normal(r, out_normal);

	return true;
}
