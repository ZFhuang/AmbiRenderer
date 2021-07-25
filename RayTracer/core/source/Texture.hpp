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

	virtual Color value(double u, double v, const Point3& p) const override;

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

	virtual Color value(double u, double v, const Point3& p) const override;

private:
	shared_ptr<Texture> odd;
	shared_ptr<Texture> even;
};

// 灰色的柏林Perlin噪声纹理
class PerlinNoiseTexture :public SolidColor {
public:
	PerlinNoiseTexture() {}
	PerlinNoiseTexture(double sc) :scale(sc) {}
	virtual Color value(double u, double v, const Point3& p) const override;

private:
	Perlin perlin;
	double scale;
};

// 图片纹理, 不属于实体纹理因此直接从Texture继承
class ImageTexture : public Texture {
public:
	// 每像素用RGB表示, 所以占三个比特
	const static int bytes_per_pixel = 3;
	// 没有图片时
	ImageTexture() :data(nullptr), width(0), height(0), bytes_per_scanline(0) {}
	// 有图片时
	ImageTexture(const char* filename);
	// 委托const char*
	ImageTexture(std::string filename);

	~ImageTexture() {
		delete data;
	}

	virtual Color value(double u, double v, const Point3& p) const override;

private:
	unsigned char* data;
	int width, height;
	// 每一行对应的总比特值
	int bytes_per_scanline;
};