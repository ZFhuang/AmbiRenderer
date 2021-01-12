#pragma once

#include "Hittable.hpp"
#include "Ray.hpp"
#include <vector>
#include <memory>

// 用到了智能指针
using std::shared_ptr;
using std::make_shared;

// 用来储存场景中所有可以被命中的对象的列表
class HittableList :public Hittable
{
public:
	HittableList() {};
	// 初始化时往列表加入目标对象的指针
	HittableList(shared_ptr<Hittable> obj) { add(obj); }
	// 清空当前列表的函数
	void clear() { objects.clear(); }
	// 压入新对象
	void add(shared_ptr<Hittable> obj) { objects.push_back(obj); }
	// 用一个vector来储存指向可命中目标的对象的指针
	std::vector<shared_ptr<Hittable>> objects;
	// 同样继承了虚函数判断是否命中
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
};

inline bool HittableList::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const {
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
