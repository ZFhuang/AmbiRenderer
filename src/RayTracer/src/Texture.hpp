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

	virtual Color value(double u, double v, const Point3& p) const override;

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

	virtual Color value(double u, double v, const Point3& p) const override;

private:
	shared_ptr<Texture> odd;
	shared_ptr<Texture> even;
};

// ��ɫ�İ���Perlin��������
class PerlinNoiseTexture :public SolidColor {
public:
	PerlinNoiseTexture() {}
	PerlinNoiseTexture(double sc) :scale(sc) {}
	virtual Color value(double u, double v, const Point3& p) const override;

private:
	Perlin perlin;
	double scale;
};

// ͼƬ����, ������ʵ���������ֱ�Ӵ�Texture�̳�
class ImageTexture : public Texture {
public:
	// ÿ������RGB��ʾ, ����ռ��������
	const static int bytes_per_pixel = 3;
	// û��ͼƬʱ
	ImageTexture() :data(nullptr), width(0), height(0), bytes_per_scanline(0) {}
	// ��ͼƬʱ
	ImageTexture(const char* filename);
	// ί��const char*
	ImageTexture(std::string filename);

	~ImageTexture() {
		delete data;
	}

	virtual Color value(double u, double v, const Point3& p) const override;

private:
	unsigned char* data;
	int width, height;
	// ÿһ�ж�Ӧ���ܱ���ֵ
	int bytes_per_scanline;
};