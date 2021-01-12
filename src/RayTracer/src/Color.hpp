#pragma once

#include "Vec3.hpp"
#include <iostream>

// 专门的color文件用来保存那些和颜色有关的函数

// 封装将pixel中的颜色分量写入对应的流out中, 通常把out重定向到文件
void write_color(std::ostream& out, Color pixel) {
	out << static_cast<int>(255.999 * pixel.x()) << ' '
		<< static_cast<int>(255.999 * pixel.y()) << ' '
		<< static_cast<int>(255.999 * pixel.z()) << '\n';
}
