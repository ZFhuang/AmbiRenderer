#include "Box.hpp"

Box::Box(const Point3& p0, const Point3& p1, shared_ptr<Material> ptr)
{
	box_min = p0;
	box_max = p1;

	sides.add(make_shared<XY_Rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
	sides.add(make_shared<XY_Rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
	sides.add(make_shared<XZ_Rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
	sides.add(make_shared<XZ_Rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
	sides.add(make_shared<YZ_Rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
	sides.add(make_shared<YZ_Rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool Box::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const
{
	return sides.hit(r,s_min, s_max, rec);
}

bool Box::bounding_box(double time0, double time1, AABB& output_box) const
{
	// 返回两个角点得到的包围盒
	output_box = AABB(box_min, box_max);
	return true;
}
