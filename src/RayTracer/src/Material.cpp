#include "Material.hpp"

bool Lambertian::scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
	// 错误的漫反射, 向量没有进行归一化, 尽管散射方向也是集中在法线附近, 但是强度不一
	//auto scatter_direction = rec.normal + random_in_unit_sphere();
	// 归一化得到正确的漫反射, 方向也是集中于法线, 但是强度是均匀的, 这里要注意
	auto scatter_direction = rec.normal + random_unit_vector();
	// 半球漫反射, 与上面是等价的, 但是效果却不太一样?
	//auto scatter_direction = random_in_hemisphere(rec.normal);
	if (scatter_direction.near_zero()) {
		// 散射新向量很接近零向量时, 直接从法线方向散射
		scatter_direction = rec.normal;
	}
	// 返回新射线, 记得要考虑时间
	scattered = Ray(rec.p, scatter_direction, in.time());
	// 应用反照率
	attenuation = albedo->value(rec.u, rec.v, rec.p);
	return true;
}

bool Metal::scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
	// 镜面反射得到新的散射方向
	Vec3 reflected = reflect(unit_vector(in.direction()), rec.normal);
	// 返回新射线, 除了镜面反射外, 通过参数控制模糊程度, 给反射加上随机扰动, 要考虑时间
	scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(), in.time());
	// 应用反照率
	attenuation = albedo;
	// 只有当反射角度和法线同侧时才认为正确反射了, 否则认为是从背面进入的射线, 不计算
	return (dot(scattered.direction(), rec.normal) > 0);
}

bool Dielectric::scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
	// 反照率暂时设置全反照但是稍微暗一点
	attenuation = Color(0.95, 0.95, 0.95);
	// 计算两种介质的折射率比例, 原始折射率设置为空气, 除新折射率, 内部则直接设置为不变
	double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
	// 将入射向量单位化
	Vec3 unit_direction = unit_vector(in.direction());
	// 这里有个问题, 在现实中如果折射率之比>1, 则折射实际上无法发生, 此时介质会变成反射
	double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
	// 因此计算出折射比例的sin
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	// 判断折射角度sin'是否大于0, 大于则需要反射
	bool cannot_refract = refraction_ratio * sin_theta > 1.0;
	// 计算出射向量方向
	Vec3 direction;
	if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
		// 反射
		direction = reflect(unit_direction, rec.normal);
	}
	else {
		// 折射
		direction = refract(unit_direction, rec.normal, refraction_ratio);
	}
	// 返回新射线, 要考虑时间
	scattered = Ray(rec.p, direction, in.time());
	return true;
}

double Dielectric::reflectance(double cosine, double ref_idx) {
	// 这里都没搞懂
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}
