﻿#pragma once

#include "Ray.hpp"
#include "Vec3.hpp"
#include "Hittable.hpp"
#include "Texture.hpp"

// 前置声明
struct HitRecord;

// 材质抽象类, 用来控制物体表面的反射特性
class Material
{
public:
	// 默认不设置的时侯材质的发光属性是黑色(无)
	virtual Color emitted(double u, double v, Point3& p) {
		return Color(0, 0, 0);
	}
	virtual bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

// 漫反射材质
class Lambertian : public Material
{
public:
	Lambertian(const Color& a) :albedo(make_shared<SolidColor>(a)) {}
	Lambertian(shared_ptr<Texture> a) :albedo(a) {}

	// 控制射线在对象上的散射效果
	virtual bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

	// 反照的材质颜色
	shared_ptr<Texture> albedo;
};

// 金属反射材质
class Metal : public Material
{
public:
	Metal(const Color& albedo, const double& fuzz = 0.0) :albedo(albedo), fuzz(fuzz) {}

	// 控制射线在对象上的反射效果
	virtual bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

	// 反照率, 控制光线能量吸收强度
	Color albedo;
	// 模糊度, 控制漫反射强度, abs(>=0), 0就是完全镜面反射
	double fuzz;
};

// 玻璃折射材质
class Dielectric :public Material {
public:
	// 设置材质折射指数
	Dielectric(double index_of_refraction) :ir(index_of_refraction) {}

	// 控制射线在对象上的折射效果
	virtual bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

	// 折射指数
	double ir;

private:
	// Schlick近似, 模拟在大角度观察折射物体时会在边缘发生的反射现象
	static double reflectance(double cosine, double ref_idx);
};

// 发光材质
class DiffuseLight : public Material {
public:
	DiffuseLight(shared_ptr<Texture> a) : emit(a) {}
	DiffuseLight(Color c) : emit(make_shared<SolidColor>(c)) {}

	// 发光材质不进行折射/反射
	virtual bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
		return false;
	}

	// 获取点对应的发光颜色
	virtual Color emitted(double u, double v, Point3& p) {
		return emit->value(u, v, p);
	}

public:
	shared_ptr<Texture> emit;
};

// 各向异性材质(例如雾)
class Isotropic : public Material {
public:
	Isotropic(Color c) :albedo(make_shared<SolidColor>(c)) {}
	Isotropic(shared_ptr<Texture> a) :albedo(a) {}

	virtual bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;
public:
	shared_ptr<Texture> albedo;
};
