#include "ConstantMedium.hpp"

bool ConstantMedium::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const
{
	// debug开关, 0.00001的几率选中一条线进行追踪
	const bool enableDebug = false;
	const bool debugging = enableDebug && random_double() < 0.00001;

	HitRecord rec1, rec2;

	// 若当前射线整个距离内都无hit, 则无
	if (!boundary->hit(r, -infinity, infinity, rec1)) {
		return false;
	}
	// 若有, 看接下来还有没有hit, 这是为了确保hit点不处于边界
	if (!boundary->hit(r, rec1.s + 0.0001, infinity, rec2)) {
		return false;
	}
	if (debugging) {
		std::cerr << "\ns_min=" << rec1.s << ", s_max=" << rec2.s << '\n';
	}
	// 限制两个hit点
	if (rec1.s < s_min) {
		rec1.s = s_min;
	}
	if (rec2.s > s_max) {
		rec2.s = s_max;
	}
	// ?不知道怎么产生的情况, 但也是边界检测
	if (rec1.s > rec2.s) {
		return false;
	}
	if (rec1.s < 0) {
		rec2.s = 0;
	}

	const auto ray_len = r.direction().length();
	// 计算两个交点间的距离, 即进入体积雾的距离
	const auto dis_inside_boundary = (rec2.s - rec1.s) * ray_len;
	// 随机发生折射
	const auto hit_dis = neg_inv_density * log(random_double());
	if (hit_dis > dis_inside_boundary) {
		return false;
	}

	// 发生折射后设置射线
	rec.s = rec1.s + hit_dis / ray_len;
	rec.p = r.at(rec.s);

	if (debugging) {
		std::cerr << "hit_distance = " << hit_dis << '\n' << "rec.t = " << rec.s << '\n' << "rec.p = " << rec.p << '\n';
	}
	// 这两个任意设置
	rec.normal = Vec3(1, 0, 0);
	rec.front_face = true;
	// 各向异性材质, 里面是随机进行折射的
	rec.mat_ptr = phase_function;

	return true;
}

bool ConstantMedium::bounding_box(double time0, double time1, AABB& output_box) const
{
	// 递归计算包围盒
	return boundary->bounding_box(time0, time1, output_box);
}
