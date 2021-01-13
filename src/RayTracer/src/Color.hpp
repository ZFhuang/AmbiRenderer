#pragma once

#include "Vec3.hpp"
#include "RTweekend.hpp"
#include <iostream>

// 专门的color文件用来保存那些和颜色有关的函数

// 封装将pixel中的颜色分量写入对应的流out中, 通常把out重定向到文件
// 增加参数sampleTimes来负责对超采样过的像素进行除法
inline void write_color(std::ostream& out, Color pixel, int sampleTimes = 1) {
	// 超采样抗锯齿是将多次采样的值叠加在一起, 所以要除法
	auto s = 1.0 / sampleTimes;
	auto r = pixel.x() * s;
	auto g = pixel.y() * s;
	auto b = pixel.z() * s;

	out << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}
