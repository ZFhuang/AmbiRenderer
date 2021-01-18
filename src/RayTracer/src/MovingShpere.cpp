#include "MovingSphere.hpp"

Point3 MovingSphere::center(double time) const {
	// 用时间作比例对位置进行线性插值
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const {
	// 特点是新的目标位置需要利用time参数来计算
	Vec3 oc = r.origin() - center(r.time());
	auto a = r.direction().length_squa();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squa() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) {
		return false;
	}
	auto sqrtd = sqrt(discriminant);

	auto root = (-half_b - sqrtd) / a;
	if (root<s_min || root>s_max) {
		root = (-half_b + sqrtd) / a;
		if (root < s_min || root > s_max) {
			return false;
		}
	}

	rec.s = root;
	rec.p = r.at(rec.s);
	// 同样需要用time来得到确切的位置
	Vec3 out_normal = (rec.p - center(r.time())) / radius;
	rec.set_face_normal(r, out_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}
