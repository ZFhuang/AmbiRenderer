#include "AABB.hpp"

AABB surrounding_box(AABB a, AABB b) {
	Point3 small(fmin(a.min().x(), b.min().x()), fmin(a.min().y(), b.min().y()), fmin(a.min().z(), b.min().z()));
	Point3 big(fmin(a.max().x(), b.max().x()), fmin(a.max().y(), b.max().y()), fmin(a.max().z(), b.max().z()));
	return AABB(small, big);
}