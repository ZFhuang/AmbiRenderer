#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

class Camera
{
public:
	Camera(
		Point3 lookfrom,
		Point3 lookat,
		Vec3 lookup,
		double aspect_ratio = 1.0,
		double v_fov = 40.0,
		double lens_radius = 0.0,
		double focus_dist = 10.0,
		double time0 = 0,
		double shutter_time = 1);
	Camera();

	// 输入视图尺度(0-1)的比例, 返回相机射出的射线
	Ray getRay(double s, double t) const;

	void setLookfrom(const Point3 lookfrom);
	void setLookat(const Point3 lookat);
	void setLookup(const Vec3 lookup);
	void setFov(const double vertical_fov);
	void setCurrentTime(const double time0);
	void setShutterTime(const double shutter_time);
	void setAspectRatio(const double aspect_ratio);
	void setFocusDist(const double focus_dist);
	void setLensRadius(const double lens_radius);

	Point3 getLookfrom() const;
	Point3 getLookat() const;
	Vec3 getLookup() const;

private:
	void _refleshViewport();
	void _refleshLocalAxis();
	void _refleshTime();
	void _refleshGlobalAxis();

private:
	/// 成员变量, 都可以用set设置, 部分可以用get读取
	Point3 m_lookfrom;
	Point3 m_lookat;
	// 相机朝上方向
	Vec3 m_lookup;
	// 画面长宽比
	double m_aspect_ratio = 0;
	// fov是视平面范围, 改变fov调整视平面的大小, 这个参数和焦长是可以互相抵消的
	// fov小相当于高放大倍率
	double m_v_fov = 0;
	// 光圈半径, 0时完美渲染
	double m_lens_radius = 0;
	// 像距
	double m_focus_dist = 0;
	// 快门打开的时间点
	double m_time0 = 0;
	// 快门时间
	double m_shutter_time = 0;

private:
	/// 内部变量, 由函数内部自动维护
	Vec3 _horizontal;
	Vec3 _vertical;
	Point3 _lower_left_corner;
	Vec3 _u, _v, _w;
	double _viewport_h = 0;
	double _viewport_w = 0;
	// 快门关闭的时间点
	double _time1 = 0;
};
