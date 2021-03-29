#pragma once
#include "Vec3.hpp"
#include "Perlin.hpp"
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
	SolidColor() {}
	SolidColor(Color c) :color_val(c) {}
	SolidColor(double r, double g, double b) :SolidColor(Color(r, g, b)) {}

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
	CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd) :even(_even), odd(_odd) {}
	CheckerTexture(Color c1, Color c2) :CheckerTexture(make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

	virtual Color value(double u, double v, const Point3& p) const override {
		// 利用sin的周期性来得到符号判断是否需要绘制格子
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

// 灰色的柏林Perlin噪声纹理
class PerlinNoiseTexture :public SolidColor {
public:
	PerlinNoiseTexture() {}
	PerlinNoiseTexture(double sc) :scale(sc) {}
	virtual Color value(double u, double v, const Point3& p) const override {
		// 输入三维点返回Perlin噪声, 与白色相乘得到灰色的柏林噪声纹理
		// 等比率改变输入坐标的值, 使得微小的坐标变化也会被映射到更大的噪声空间中
		// 因此可以起到增大噪声频率的效果
		// 这里用到了湍流效果, 增强了噪声间的差异, 但是很多时候湍流并不会直接使用
		// 而是作为噪声放到sin函数之类的周期函数中来间接影响颜色, 达到周期变化的效果
		// 由于sin有负数, 记得归一化来使得gamma校正步骤不出问题
		return Color(1, 1, 1) *0.5*(1.0+sin(scale*p.z()+10*perlin.turbulence(p)));
	}

private:
	Perlin perlin;
	double scale;
};