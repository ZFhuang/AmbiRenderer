#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

class Camera
{
public:
	Camera(double aspect_ratio = 16.0 / 9.0, double viewport_height = 2.0, double focal_length = 1.0) {
		double viewport_width = aspect_ratio * viewport_height;

		// 相机当前位置
		origin = Point3(0, 0, 0);
		// 水平方向的长度向量
		horizontal = Vec3(viewport_width, 0, 0);
		// 垂直方向的长度向量
		vertical = Vec3(0, viewport_height, 0);
		// 计算出视平面左下角的真实位置
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
	}

	Ray getRay(double u, double v) const {
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

private:
	Point3 origin;
	Vec3 horizontal;
	Vec3 vertical;
	Point3 lower_left_corner;
};
