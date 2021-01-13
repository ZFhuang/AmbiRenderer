#include "HittableList.hpp"

bool HittableList::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const {
	HitRecord tmp_rec;
	// 命中任何一个对象都会返回true
	bool hit_any = false;
	auto closest = s_max;
	// 遍历所有列表中的对象
	for (const auto& obj : objects) {
		// 调用对象本身的命中函数来判断, 这里注意只判断那些可能更近的命中
		if (obj->hit(r, s_min, closest, tmp_rec)) {
			hit_any = true;
			// 这里储存更近的一个命中的距离
			closest = tmp_rec.s;
			rec = tmp_rec;
		}
	}
	return hit_any;
}
