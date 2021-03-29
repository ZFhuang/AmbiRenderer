#pragma once
#include "Vec3.hpp"
#include "Perlin.hpp"
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
	SolidColor() {}
	SolidColor(Color c) :color_val(c) {}
	SolidColor(double r, double g, double b) :SolidColor(Color(r, g, b)) {}

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
	CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd) :even(_even), odd(_odd) {}
	CheckerTexture(Color c1, Color c2) :CheckerTexture(make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

	virtual Color value(double u, double v, const Point3& p) const override {
		// ����sin�����������õ������ж��Ƿ���Ҫ���Ƹ���
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0) {
			return odd->value(u, v, p);
		}
		else {
			return even->value(u, v, p);
		}
	}

private:
	shared_ptr<Texture> odd;
	shared_ptr<Texture> even;
};

// ��ɫ�İ���Perlin��������
class PerlinNoiseTexture :public SolidColor {
public:
	PerlinNoiseTexture() {}
	PerlinNoiseTexture(double sc) :scale(sc) {}
	virtual Color value(double u, double v, const Point3& p) const override {
		// ������ά�㷵��Perlin����, ���ɫ��˵õ���ɫ�İ�����������
		// �ȱ��ʸı����������ֵ, ʹ��΢С������仯Ҳ�ᱻӳ�䵽����������ռ���
		// ��˿�������������Ƶ�ʵ�Ч��
		// �����õ�������Ч��, ��ǿ��������Ĳ���, ���Ǻܶ�ʱ������������ֱ��ʹ��
		// ������Ϊ�����ŵ�sin����֮������ں����������Ӱ����ɫ, �ﵽ���ڱ仯��Ч��
		// ����sin�и���, �ǵù�һ����ʹ��gammaУ�����費������
		return Color(1, 1, 1) *0.5*(1.0+sin(scale*p.z()+10*perlin.turbulence(p)));
	}

private:
	Perlin perlin;
	double scale;
};