#pragma once
#include "rtw_stb_image.hpp"
#include "Texture.hpp"

Color SolidColor::value(double u, double v, const Point3& p) const {
	return color_val;
}

Color CheckerTexture::value(double u, double v, const Point3& p) const {
	// 利用sin的周期性来得到符号判断是否需要绘制格子
	auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
	if (sines < 0) {
		return odd->value(u, v, p);
	}
	else {
		return even->value(u, v, p);
	}
}

Color PerlinNoiseTexture::value(double u, double v, const Point3& p) const {
	// 输入三维点返回Perlin噪声, 与白色相乘得到灰色的柏林噪声纹理
	// 等比率改变输入坐标的值, 使得微小的坐标变化也会被映射到更大的噪声空间中
	// 因此可以起到增大噪声频率的效果
	// 这里用到了湍流效果, 增强了噪声间的差异, 但是很多时候湍流并不会直接使用
	// 而是作为噪声放到sin函数之类的周期函数中来间接影响颜色, 达到周期变化的效果
	// 由于sin有负数, 记得归一化来使得gamma校正步骤不出问题
	return Color(1, 1, 1) * 0.5 * (1.0 + sin(scale * p.z() + 10 * perlin.turbulence(p)));
}

ImageTexture::ImageTexture(const char* filename) {
	auto components_per_pixel = bytes_per_pixel;
	// 读取图片到data数组中
	data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
	if (!data) {
		std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
		width = height = 0;
	}
	// 计算行比特数, 用来索引data数组
	bytes_per_scanline = bytes_per_pixel * width;
}

Color ImageTexture::value(double u, double v, const Point3& p) const {
	// 没有图片时返回固定值
	if (data == nullptr) {
		return Color(0, 1, 1);
	}
	// 缩放模型坐标uv到(0,1)
	u = clamp(u, 0.0, 1.0);
	v = 1.0 - clamp(v, 0.0, 1.0);
	// 重新将uv映射到图片像素上
	auto i = static_cast<int>(u * width);
	auto j = static_cast<int>(v * height);
	if (i >= width) {
		i = width - 1;
	}
	if (j >= height) {
		j = height - 1;
	}
	const auto color_scale = 1.0 / 255.0;
	// 从数组中索引取值并返回
	auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
	return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}