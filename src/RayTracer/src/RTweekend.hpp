#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "Ray.hpp"
#include "Vec3.hpp"

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
	static std::uniform_real_distribution<double> distribution(min, max);
	static std::mt19937 generator;
	return distribution(generator);
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
