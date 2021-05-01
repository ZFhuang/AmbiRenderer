#pragma once
#include "rtw_stb_image.hpp"
#include "Texture.hpp"

Color SolidColor::value(double u, double v, const Point3& p) const {
	return color_val;
}

Color CheckerTexture::value(double u, double v, const Point3& p) const {
	// ����sin�����������õ������ж��Ƿ���Ҫ���Ƹ���
	auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
	if (sines < 0) {
		return odd->value(u, v, p);
	}
	else {
		return even->value(u, v, p);
	}
}

Color PerlinNoiseTexture::value(double u, double v, const Point3& p) const {
	// ������ά�㷵��Perlin����, ���ɫ��˵õ���ɫ�İ�����������
	// �ȱ��ʸı����������ֵ, ʹ��΢С������仯Ҳ�ᱻӳ�䵽����������ռ���
	// ��˿�������������Ƶ�ʵ�Ч��
	// �����õ�������Ч��, ��ǿ��������Ĳ���, ���Ǻܶ�ʱ������������ֱ��ʹ��
	// ������Ϊ�����ŵ�sin����֮������ں����������Ӱ����ɫ, �ﵽ���ڱ仯��Ч��
	// ����sin�и���, �ǵù�һ����ʹ��gammaУ�����費������
	return Color(1, 1, 1) * 0.5 * (1.0 + sin(scale * p.z() + 10 * perlin.turbulence(p)));
}

ImageTexture::ImageTexture(const char* filename) {
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

Color ImageTexture::value(double u, double v, const Point3& p) const {
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