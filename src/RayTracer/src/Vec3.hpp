#pragma once

#include <iostream>

#include "RTweekend.hpp"

using std::sqrt;

// 图形程序中不可或缺的三维向量(3D Vector)类, 由内联函数组成
// 核心是包装一个小小的数组并用大量的重载运算符包裹为新的数据结构
class Vec3 {
public:
	// 向量的默认构造, 得到一个零向量
	Vec3() : e{ 0,0,0 } {}

	// 可以选择需要初始化的分量值
	Vec3(double e1, double e2, double e3) :e{ e1,e2,e3 } {}

	// 这里用到了常量成员函数, 不能在此修改e的内容, 用来访问e
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// 重载一元负号运算符, 返回负vec的拷贝, 和平时用到的负号行为一致
	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

	// 重载两个版本的下标运算符
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	// 重载加号
	Vec3& operator+=(const Vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	// 重载乘号, 这里是统一的缩放
	Vec3& operator*=(const double s) {
		e[0] *= s;
		e[1] *= s;
		e[2] *= s;
		return *this;
	}

	// 重载除法, 用乘1/s实现
	Vec3& operator/=(const double s) {
		return (*this) *= (1 / s);
	}

	// 返回向量长度
	double length() const {
		return sqrt(length_squa());
	}

	// 返回向量模的平方
	double length_squa() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// 生成随机向量
	inline static Vec3 random(double min=0.0, double max=1.0) {
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

private:
	// 储存向量值的数组
	double e[3];
};

// 用using重命名Vec3
using Point3 = Vec3;
// 用using重命名Vec3
using Color = Vec3;

// 一些有关Vec3的工具函数, 在外部内联

// 重载流运算符, 方便直接将向量写入文件
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

// 重载加号, 返回两个向量相加后的拷贝
inline Vec3 operator+(const Vec3& u, const Vec3& v) {
	return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

// 重载减号, 返回两个向量相减后的拷贝
inline Vec3 operator-(const Vec3& u, const Vec3& v) {
	return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

// 重载乘号, 返回两个向量按分量乘后的拷贝
inline Vec3 operator*(const Vec3& u, const Vec3& v) {
	return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

// 重载乘号, 返回向量数乘后的拷贝
inline Vec3 operator*(double s, const Vec3& v) {
	return Vec3(s * v.x(), s * v.y(), s * v.z());
}

// 重载乘号另一个顺序, 返回向量数乘后的拷贝
inline Vec3 operator*(const Vec3& v, double s) {
	return s * v;
}

// 重载除号, 除号只有一种顺序
inline Vec3 operator/(Vec3 v, double s) {
	return (1 / s) * v;
}

// 向量点乘函数, 得到两个向量对应分量相乘后的求和, 结果是常数, u * v'
inline double dot(const Vec3& u, const Vec3& v) {
	return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

// 叉乘函数, 结果是垂直两者的向量, 是求解行列式|ijk-u1u2u3-v1v2v3|, u x v
inline Vec3 cross(const Vec3& u, const Vec3& v) {
	return Vec3(
		u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x()
	);
}

// 返回单位向量形式的向量v, 也就是除了自己的模
inline Vec3 unit_vector(const Vec3& v) {
	return v / v.length();
}

// 用来模拟漫反射的随机向量, 在单位圆内随机分布, 直接使用的话会使得分布比较集中于中心
inline Vec3 random_in_unit_sphere() {
	// 反复生成
	while (true) {
		auto p = Vec3::random(-1.0, 1.0);
		// 接受-拒绝采样
		if (p.length_squa() >= 1) {
			continue;
		}
		return p;
	}
}

// 真正的理想漫反射方程是在法线半球上改变而不是叠加分量, 主要问题在于叠加分量的话会导致
// 反射光溢出, 能量超过cos. 正确的做法就是直接在半球上采样然后使用
inline Vec3 random_in_hemisphere(const Vec3& normal) {
	Vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) {
		return in_unit_sphere;
	}
	else {
		return -in_unit_sphere;
	}
}

// 将用来模拟漫反射的单位圆随机向量单位向量化, 也就是只取圆上的点了, 让向量分布更加均匀
// 可想像一下, 单位化后一个切线球上的采样效果类似于一个切半球, 且是均匀分布的
inline Vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}
