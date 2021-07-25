#include "Camera.hpp"

Camera::Camera() :Camera(Point3(1, 0, 0), Point3(0, 0, 0), Point3(0, 1, 0))
{
	// C11的委托构造
}

Camera::Camera(
	Point3 lookfrom,
	Point3 lookat,
	Vec3 lookup,
	double aspect_ratio,
	double v_fov,
	double lens_radius,
	double focus_dist,
	double time0,
	double shutter_time)
{
	m_lookfrom = lookfrom;
	m_lookat = lookat;
	m_lookup = lookup;
	m_aspect_ratio = aspect_ratio;
	m_v_fov = v_fov;
	m_lens_radius = lens_radius;
	m_focus_dist = focus_dist;
	m_time0 = time0;
	m_shutter_time = shutter_time;

	_refleshTime();
	_refleshViewport();
	_refleshLocalAxis();
	_refleshGlobalAxis();
}

Ray Camera::getRay(double s, double t) const
{
	// 光圈不为0时, 在光圈圆盘上随机采样点作为新的成像点, 用那个点成像
	// 因此光圈越大, 成像越不准确(模糊)
	Vec3 offset;
	// 由于随机数是在(-1,1)取值, 因此需要用除二的半径来当倍率
	Vec3 rd = m_lens_radius * random_in_unit_disk();
	offset = _u * rd.x() + _v * rd.y();
	// 加入时间因素
	return Ray(m_lookfrom + offset, _lower_left_corner + s * _horizontal + t * _vertical - m_lookfrom - offset, random_double(m_time0, _time1));
}

void Camera::setLookfrom(const Point3 lookfrom) {
	m_lookfrom = lookfrom;
	_refleshLocalAxis();
}

void Camera::setLookat(const Point3 lookat) {
	m_lookat = lookat;
	_refleshLocalAxis();
}

void Camera::setLookup(const Vec3 lookup) {
	m_lookup = lookup;
	_refleshLocalAxis();
}

void Camera::setFov(const double v_fov)
{
	// 实际上就是假定焦长为1.0时, 上下边与相机中心形成的角度
	// 单位是角度因此转为弧度方便计算
	m_v_fov = v_fov;
	_refleshViewport();
}

void Camera::setCurrentTime(const double time0)
{
	m_time0 = time0;
	_refleshTime();
}

void Camera::setShutterTime(const double shutter_time)
{
	m_shutter_time = shutter_time;
	_refleshTime();
}

void Camera::setAspectRatio(const double aspect_ratio)
{
	m_aspect_ratio = aspect_ratio;
	_refleshViewport();
}

void Camera::setFocusDist(const double focus_dist)
{
	m_focus_dist = focus_dist;
	_refleshGlobalAxis();
}

void Camera::setLensRadius(const double lens_radius)
{
	m_lens_radius = lens_radius;
}

Point3 Camera::getLookfrom() const {
	return m_lookfrom;
}

Point3 Camera::getLookat() const {
	return m_lookat;
}

Vec3 Camera::getLookup() const {
	return m_lookup;
}

void Camera::_refleshViewport()
{
	auto theta = degrees_to_radians(m_v_fov);
	// 用一半的弧度计算出一半的视平面高度, 此时相当于原点在中心
	auto h = tan(theta / 2);
	// 乘2得到视平面的高度(相当于在左下角)
	_viewport_h = 2.0 * h;
	// 通过长宽比得到宽度(相当于在左下角)
	_viewport_w = m_aspect_ratio * _viewport_h;
	_refleshGlobalAxis();
}

void Camera::_refleshLocalAxis()
{
	// w是相机的朝向向量
	_w = unit_vector(m_lookfrom - m_lookat);
	// u是根据相机朝上的向量与朝向叉乘得到的水平轴向量
	// 朝上的向量和真实的视平面垂直向量并不平行, 但是会在同一个面上
	_u = unit_vector(cross(m_lookup, _w));
	// 再利用朝向和水平向量叉乘得到真正的视平面垂直向量
	_v = cross(_w, _u);
	_refleshGlobalAxis();
}

void Camera::_refleshTime()
{
	_time1 = m_time0 + m_shutter_time;
}

void Camera::_refleshGlobalAxis()
{
	// 下面从比例转为计算轴向量
	// 这里通过乘上focus_dist使得在这个距离的物体光圈的随机采样会应用在同个像素上
	// 水平方向的轴向量进行缩放得到真正的轴
	_horizontal = m_focus_dist * _viewport_w * _u;
	// 垂直方向的轴
	_vertical = m_focus_dist * _viewport_h * _v;
	// 配合焦长平移视平面, 得到左下角的真实位置, 也就是平面坐标系的原点
	_lower_left_corner = m_lookfrom - _horizontal / 2 - _vertical / 2 - m_focus_dist * _w;
}
