#pragma once
#include "Vec3.hpp"
#include <memory>

// �������
class Texture {
public:
	// uv�Ƕ�Ӧ�Ĳ�������, p����ά���е�
	virtual Color value(double u, double v, const Point3& p) const = 0;
};

// �ɳ���ʵʱ���ɵ�ʵ��������, �̳��������, Ҳ������Ϊ���������������̳�
class SolidColor :public Texture {
public:
	SolidColor(){}
	SolidColor(Color c):color_val(c){}
	SolidColor(double r,double g,double b):SolidColor(Color(r,g,b)){}

	virtual Color value(double u, double v, const Point3& p) const override {
		return color_val;
	}

private:
	Color color_val;
};

// ���̸�����, ����ʵ�������һ����˼̳���SolidColor
class CheckerTexture :public SolidColor {
public:
	CheckerTexture() {}
	// ����ż�����Ʒ��ص�����
	CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd) :even(_even),odd(_odd) {}
	CheckerTexture(Color c1, Color c2) :CheckerTexture(make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

	virtual Color value(double u, double v, const Point3& p) const override {
		// ����sin�����������õ������ж��Ƿ���Ҫ���Ƹ���
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0) {
			return odd->value(u,v,p);
		}
		else {
			return even->value(u, v, p);
		}
	}

private:
	shared_ptr<Texture> odd;
	shared_ptr<Texture> even;
};