#pragma once
#include "Vec3.hpp"
#include <memory>

// 纹理基类
class Texture {
public:
	// uv是对应的参数表面, p是三维命中点
	virtual Color value(double u, double v, const Point3& p) const = 0;
};

// 由程序实时生成的实体纹理类, 继承纹理基类, 也用来作为基类给其他纹理类继承
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

// 棋盘格纹理, 属于实体纹理的一种因此继承了SolidColor
class CheckerTexture :public SolidColor {
public:
	CheckerTexture() {}
	// 用奇偶来控制返回的棋盘
	CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd) :even(_even),odd(_odd) {}
	CheckerTexture(Color c1, Color c2) :CheckerTexture(make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

	virtual Color value(double u, double v, const Point3& p) const override {
		// 利用sin的周期性来得到符号判断是否需要绘制格子
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