#include "Hittable.hpp"

bool Translate::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const
{
	// 思路不是改变盒子的位置, 而是每次需要对盒子判断是否光线命中时
	// 对光线方向进行修改, 将光线移动到与盒子原始位置一样的地方
	Ray move_r(r.origin() - offset, r.direction(), r.time());
	if (!ptr->hit(move_r, s_min, s_max, rec)) {
		return false;
	}
	// 得到的命中点在原始位置上, 因此又偏移到新的位置上
	rec.p += offset;
	// 设置更新后的法线方向
	rec.set_face_normal(move_r, rec.normal);
	return true;
}

bool Translate::bounding_box(double time0, double time1, AABB& output_box) const
{
	if (!ptr->bounding_box(time0, time1, output_box)) {
		return false;
	}
	// 同样, 当需要得到盒子包围盒时, 利用包装返回位置更新后的包围盒
	output_box = AABB(output_box.min() + offset, output_box.max() + offset);
	return true;
}

RotateY::RotateY(shared_ptr<Hittable> p, double angle) : ptr(p)
{
	// 初始化角度和盒子, 旋转和平移不同点在于需要计算旋转后的盒子形成的新的大的轴对称包围盒
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);
	// 以下是为了计算新的包围盒
	Point3 min(infinity, infinity, infinity);
	Point3 max(-infinity, -infinity, -infinity);
	// 遍历盒子的八个点
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				// 得到当前的角点
				auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
				auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
				auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

				// 旋转角点的x和z坐标
				auto newx = cos_theta * x + sin_theta * z;
				auto newz = -sin_theta * x + cos_theta * z;

				// 组合得到新的角点
				Vec3 tester(newx, y, newz);
				// 计算更新包围盒的角点
				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}
	// 最终更新角点
	bbox = AABB(min, max);
}

bool RotateY::hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const
{
	// 判断命中情况, 光线需要反向旋转, 法线则是正常旋转, 注意下面的公式的对称性
	auto origin = r.origin();
	auto direction = r.direction();

	origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
	origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

	direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
	direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

	Ray rotate_r(origin, direction, r.time());

	if (!ptr->hit(rotate_r, s_min, s_max, rec)) {
		return false;
	}
	auto p = rec.p;
	auto normal = rec.normal;

	p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
	p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

	normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
	normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

	rec.p = p;
	rec.set_face_normal(rotate_r, normal);

	return true;
}

bool RotateY::bounding_box(double time0, double time1, AABB& output_box) const
{
	output_box = bbox;
	return hasbox;
}
