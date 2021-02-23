#pragma once
#include "Vec3.hpp"
#include "Ray.hpp"

class AABB {
public:
	AABB() {}
	// 方盒, 最大点和最小点
	AABB(const Point3& minp, const Point3& maxp) {
		minimum = minp;
		maximum = maxp;
	}

	Point3 min() const {
		return minimum;
	}

	Point3 max() const {
		return maximum;
	}

	// 判断当前射线是否命中当前包围盒, 重点是需要射线在三个轴上都有重叠Overlap部分, 见2-P16
	bool hit(const Ray& r, double t_min, double t_max) const {
		// 对x, y, z
		for (int a = 0; a < 3; ++a) {
			// 减少除法运算的优化写法
			auto invD=1.0f/ r.direction()[a];
			auto t0 = (minimum[a] - r.origin()[a]) * invD;
			auto t1 = (maximum[a] - r.origin()[a]) * invD;
			if (invD < 0.0f)
				std::swap(t0, t1);
			// 这一步用于更新当前的重叠部分, 随着判断的轴变多, t_min和t_max会越来越接近
			// 用三元运算符代替min计算或者if计算
			t_min = t0>t_min?t0:t_min;
			t_max = t1 > t_max ? t1 : t_max;
			// 若互相超过则说明有一轴不重叠了
			if (t_max <= t_min) 
				return false;
		}
		return true;
	}

	Point3 minimum;
	Point3 maximum;
};

// 从两个包围盒中返回能够容纳两个盒的最小包围盒
AABB surrounding_box(AABB a, AABB b);