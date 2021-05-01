#pragma once
#include "Vec3.hpp"
#include "Perlin.hpp"
#include "rtw_stb_image.hpp"
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
		return Color(1, 1, 1) * 0.5 * (1.0 + sin(scale * p.z() + 10 * perlin.turbulence(p)));
	}

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
	ImageTexture(const char* filename) {
		auto components_per_pixel = bytes_per_pixel;
		// ��ȡͼƬ��data������
		data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
		if (!data) {
			std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
			width = height = 0;
		}
		// �����б�����, ��������data����
		bytes_per_scanline = bytes_per_pixel * width;
	}

	~ImageTexture() {
		delete data;
	}

	virtual Color value(double u, double v, const Point3& p) const override {
		// û��ͼƬʱ���ع̶�ֵ
		if (data == nullptr) {
			return Color(0, 1, 1);
		}
		// ����ģ������uv��(0,1)
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0);
		// ���½�uvӳ�䵽ͼƬ������
		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);
		if (i >= width) {
			i = width - 1;
		}
		if (j >= height) {
			j = height - 1;
		}
		const auto color_scale = 1.0 / 255.0;
		// ������������ȡֵ������
		auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
		return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}

private:
	unsigned char* data;
	int width, height;
	// ÿһ�ж�Ӧ���ܱ���ֵ
	int bytes_per_scanline;
};