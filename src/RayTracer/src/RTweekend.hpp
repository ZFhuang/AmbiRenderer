#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// 保存Ray Tracing in One Weekend项目所需的基本常量和函数

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// 角度转弧度
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

// 生成范围内的double均匀分布随机数
inline double random_double(double min = 0.0, double max = 1.0) {
	// 生成种子
	static std::random_device rd;
	// 调用生成器
	static std::mt19937 generator(rd());
	// 注意这个该死的生成器只能生成非负数
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	// 移动分布范围
	return min + (max - min) * distribution(generator);
}

inline int random_int(int min = 0, int max = 1) {
	return static_cast<int>(random_double(min, max + 1));
}

// 按照min和max对值进行截断
inline double clamp(double x, double min, double max) {
	if (x < min) {
		return min;
	}
	if (x > max) {
		return max;
	}

	return x;
}
