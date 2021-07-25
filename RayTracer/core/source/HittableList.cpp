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

// 将当前对象列表中的所有对象依次加入包围盒中得到一个最大的包围盒
bool HittableList::bounding_box(double time0, double time1, AABB& output_box) const {
	// 空列表无盒
	if (objects.empty()) {
		return false;
	}
	AABB temp_box;
	bool first_box = true;
	for (const auto& obj : objects) {
		// 有一个对象无盒则列表无盒
		if (!obj->bounding_box(time0, time1, temp_box)) {
			return false;
		}
		// 初始化则使用空盒, 否则将新的盒包含进当前盒然后更新
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}
	return true;
}
