#pragma once

#include<cmath>
#include<iostream>

using std::sqrt;

// ͼ�γ����в��ɻ�ȱ����ά����(3D Vector)��, �������������
// �����ǰ�װһ��СС�����鲢�ô������������������Ϊ�µ����ݽṹ
class Vec3 {
public:
	// ������Ĭ�Ϲ���, �õ�һ��������
	Vec3() : e{ 0,0,0 } {}

	// ����ѡ����Ҫ��ʼ���ķ���ֵ
	Vec3(double e1, double e2, double e3) :e{ e1,e2,e3 } {}

	// �����õ��˳�����Ա����, �����ڴ��޸�e������, ��������e
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// ����һԪ���������, ���ظ�vec�Ŀ���, ��ƽʱ�õ��ĸ�����Ϊһ��
	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

	// ���������汾���±������
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	// ���ؼӺ�
	Vec3 operator+=(const Vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	// ���س˺�, ������ͳһ������
	Vec3 operator*=(const double s) {
		e[0] *= s;
		e[1] *= s;
		e[2] *= s;
		return *this;
	}

	// ���س���, �ó�1/sʵ��
	Vec3 operator/=(const double s) {
		return *this *= 1 / s;
	}

	// ������������
	double length() const {
		return sqrt(length_squa());
	}

private:
	// ��������ֵ������
	double e[3];

	// ��������ģ��ƽ��
	double length_squa() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
};

// ��using������Vec3
using Point3 = Vec3;
// ��using������Vec3
using Color = Vec3;

// һЩ�й�Vec3�Ĺ��ߺ���, ���ⲿ����

// �����������, ����ֱ�ӽ�����д���ļ�
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

// ���ؼӺ�, ��������������Ӻ�Ŀ���
inline Vec3 operator+(const Vec3& u, const Vec3& v) {
	return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

// ���ؼ���, �����������������Ŀ���
inline Vec3 operator-(const Vec3& u, const Vec3& v) {
	return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

// ���س˺�, �������������������˺�Ŀ���
inline Vec3 operator*(const Vec3& u, const Vec3& v) {
	return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

// ���س˺�, �����������˺�Ŀ���
inline Vec3 operator*(double s, const Vec3& v) {
	return Vec3(s * v.x(), s * v.y(), s * v.z());
}

// ���س˺���һ��˳��, �����������˺�Ŀ���
inline Vec3 operator*(const Vec3& v, double s) {
	return s * v;
}

// ���س���, ����ֻ��һ��˳��
inline Vec3 operator/(const Vec3& v, double s) {
	return v / s;
}

// ������˺���, �õ�����������Ӧ������˺�����, ����ǳ���, u * v'
inline double dot(const Vec3& u, const Vec3& v) {
	return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

// ��˺���, ����Ǵ�ֱ���ߵ�����, ���������ʽ|ijk-u1u2u3-v1v2v3|, u x v
inline Vec3 cross(const Vec3& u, const Vec3& v) {
	return Vec3(
		u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x()
	);
}

// ���ص�λ������ʽ������v, Ҳ���ǳ����Լ���ģ
inline Vec3 uint_vector(Vec3 v) {
	return v / v.length();
}