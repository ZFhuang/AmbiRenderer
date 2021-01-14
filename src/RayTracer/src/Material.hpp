﻿#pragma once

#include "Ray.hpp"
#include "Vec3.hpp"
#include "Hittable.hpp"

// 前置声明
struct HitRecord;

// 材质抽象类, 用来控制物体表面的反射特性
class Material
{
public:
	virtual bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

// 漫反射材质
class Lambertian : public Material
{
public:
	Lambertian(const Color& albedo) :albedo(albedo) {}

	// 控制射线在对象上的散射效果
	virtual bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;

	// 反照率, 控制光线能量吸收强度
	Color albedo;
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
