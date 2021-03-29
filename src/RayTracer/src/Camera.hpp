#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

class Camera
{
public:
	// 现实中的相机一半则是通过调整焦长来达到改变fov的效果, 长焦的fov小, 就有放大的效果
	Camera(
		Point3 lookfrom,
		Point3 lookat,
		Vec3 vup,	// 设置相机朝上的方向, 实际上用来约束相机左右转动的方向
		double fov_vertical,
		double aspect_ratio,
		double focus_dist, // 像距
		double aperture = 0, // 光圈大小, 0的时候是完美渲染, 越大则越虚
		double _time0 = 0,	// 相机记录此次快门打开的时间
		double _time1 = 0	// 快门关闭的时间
	) {
		// fov是视平面范围, 改变fov调整视平面的大小, 这个参数和焦长是可以互相抵消的
		// 实际上就是假定焦长为1.0时, 上下边与相机中心形成的角度
		// 单位是角度因此转为弧度方便计算
		auto theta = degrees_to_radians(fov_vertical);
		// 用一半的弧度计算出一半的视平面高度, 此时相当于原点在中心
		auto h = tan(theta / 2);
		// 乘2得到视平面的高度(相当于在左下角)
		auto viewport_height = 2.0 * h;
		// 通过长宽比得到宽度(相当于在左下角)
		double viewport_width = aspect_ratio * viewport_height;

		// w是相机的朝向向量
		w = unit_vector(lookfrom - lookat);
		// u是根据相机朝上的向量与朝向叉乘得到的水平轴向量
		// 朝上的向量和真实的视平面垂直向量并不平行, 但是会在同一个面上
		u = unit_vector(cross(vup, w));
		// 再利用朝向和水平向量叉乘得到真正的视平面垂直向量
		v = cross(w, u);

		// 下面从比例转为计算轴向量
		// 相机当前位置
		origin = lookfrom;
		// 这里通过乘上focus_dist使得在这个距离的物体光圈的随机采样会应用在同个像素上
		// 水平方向的轴向量进行缩放得到真正的轴
		horizontal = focus_dist * viewport_width * u;
		// 垂直方向的轴
		vertical = focus_dist * viewport_height * v;
		// 配合焦长平移视平面, 得到左下角的真实位置, 也就是平面坐标系的原点
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
		// 光圈半径
		lens_radius = aperture / 2;
		time0 = _time0;
		time1 = _time1;
	}

	// 输入视图的(0-1)比例, 返回相机射出的射线
	Ray getRay(double s, double t) const {
		// 光圈不为0时, 在光圈圆盘上随机采样点作为新的成像点, 用那个点成像
		// 因此光圈越大, 成像越不准确(模糊)
		Vec3 offset;
		// 由于随机数是在(-1,1)取值, 因此需要用除二的半径来当倍率
		Vec3 rd = lens_radius * random_in_unit_disk();
		offset = u * rd.x() + v * rd.y();
		// 加入时间因素
		return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_double(time0, time1));
	}

private:
	Point3 origin;
	Vec3 horizontal;
	Vec3 vertical;
	Point3 lower_left_corner;
	Vec3 u, v, w;
	// 光圈半径
	double lens_radius;
	// 快门打开的时间点
	double time0;
	// 快门关闭的时间点
	double time1;
};
