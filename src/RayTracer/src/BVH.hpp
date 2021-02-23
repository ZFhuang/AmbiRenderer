#pragma once

//#include "RTweekend.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"

// ��ΰ�Χ�н��, ʵ���Ͼ��Ƕ������ṹ
class BVH_Node :public Hittable {
public:
	BVH_Node();
	BVH_Node(const HittableList& list, double time0, double time1) :BVH_Node(list.objects,0,list.objects.size(),time0,time1){}
	BVH_Node(
		const std::vector<shared_ptr<Hittable>>& src_objects,
		size_t start,
		size_t end,
		double time0,
		double time1
		);
	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

private:
	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	AABB box;
};

// �Ƚ���, ������sort����ʹ��, �Ƚ�������Χ���������ϵ�����
inline bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis);
bool box_x_compare(const shared_ptr<Hittable> a, shared_ptr<Hittable> b);
bool box_y_compare(const shared_ptr<Hittable> a, shared_ptr<Hittable> b);
bool box_z_compare(const shared_ptr<Hittable> a, shared_ptr<Hittable> b);