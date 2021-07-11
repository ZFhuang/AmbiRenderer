#pragma once

#include "RTweekend.hpp"
#include "Hittable.hpp"
#include "Material.hpp"
#include "Texture.hpp"

class ConstantMedium :public Hittable {
public:
	ConstantMedium(shared_ptr<Hittable> b, double d, shared_ptr<Texture> a) :boundary(b), neg_inv_density(-1 / d), phase_function(make_shared<Isotropic>(a)) {}
	ConstantMedium(shared_ptr<Hittable> b, double d, Color c) :boundary(b), neg_inv_density(-1 / d), phase_function(make_shared<Isotropic>(c)) {}

	virtual bool hit(const Ray& r, double s_min, double s_max, HitRecord& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;
public:
	shared_ptr<Hittable> boundary;
	shared_ptr<Material> phase_function;
	double neg_inv_density;
};
