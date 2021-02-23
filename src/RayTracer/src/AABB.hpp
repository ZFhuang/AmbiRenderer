#pragma once
#include "Vec3.hpp"
#include "Ray.hpp"

class AABB {
public:
	AABB() {}
	// ����, �������С��
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

	// �жϵ�ǰ�����Ƿ����е�ǰ��Χ��, �ص�����Ҫ�������������϶����ص�Overlap����, ��2-P16
	bool hit(const Ray& r, double t_min, double t_max) const {
		// ��x, y, z
		for (int a = 0; a < 3; ++a) {
			// ���ٳ���������Ż�д��
			auto invD=1.0f/ r.direction()[a];
			auto t0 = (minimum[a] - r.origin()[a]) * invD;
			auto t1 = (maximum[a] - r.origin()[a]) * invD;
			if (invD < 0.0f)
				std::swap(t0, t1);
			// ��һ�����ڸ��µ�ǰ���ص�����, �����жϵ�����, t_min��t_max��Խ��Խ�ӽ�
			// ����Ԫ���������min�������if����
			t_min = t0>t_min?t0:t_min;
			t_max = t1 > t_max ? t1 : t_max;
			// �����೬����˵����һ�᲻�ص���
			if (t_max <= t_min) 
				return false;
		}
		return true;
	}

	Point3 minimum;
	Point3 maximum;
};

// ��������Χ���з����ܹ����������е���С��Χ��
AABB surrounding_box(AABB a, AABB b);