#include "BVH.hpp"
#include <algorithm>

// 建树, 最关键的一步, 这是一个递归函数
BVH_Node::BVH_Node(const std::vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end, double time0, double time1)
{
	// 获得一个没有const的当前对象表
	auto objects = src_objects;

	// 随机挑选一个切分空间的坐标轴
	int axis = random_int(0, 2);
	// 获得对应的轴比较的函数指针
	auto comparator = (axis == 0) ? &box_x_compare
		: (axis == 1) ? &box_y_compare
		: &box_z_compare;
	// 对象的数量
	size_t object_span = end - start;

	if (object_span == 1) {
		// 只有一个对象时, 指针相同
		left = right = objects[start];
	}
	else if (object_span == 2) {
		// 两个时, 比较轴向位置, 小左大右
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else {
		// 多个时, 先对物体按照对应的轴进行排序
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		// 找到序号在中间的物体
		auto mid = start + object_span / 2;
		// 左右递归建树
		left = make_shared<BVH_Node>(objects, start, mid, time0, time1);
		right = make_shared<BVH_Node>(objects, mid, end, time0, time1);
	}

	AABB box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)) {
		std::cerr << "No bounding box in bvh_node constructor.\n";
	}

	// 最后将当前结点的最小包围盒写入代表当前结点包围盒的box
	box = surrounding_box(box_left, box_right);
}

// 和二叉树一样, 利用hit与否来左右遍历
bool BVH_Node::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const
{
	if (!box.hit(r, s_min, s_max)) {
		return false;
	}

	bool hit_left = left->hit(r, s_min, s_max, rec);
	bool hit_right = right->hit(r, s_min, s_max, rec);

	return hit_left || hit_right;
}

// 返回当前结点的box
bool BVH_Node::bounding_box(double time0, double time1, AABB& output_box) const
{
	output_box = box;
	return true;
}

inline bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis)
{
	AABB box_a;
	AABB box_b;
	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)) {
		std::cerr << "No bounding box in bvh_node constructor.\n";
	}
	// 比较两个包围盒在轴向上的最小值
	return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
	return box_compare(a, b, 2);
}
