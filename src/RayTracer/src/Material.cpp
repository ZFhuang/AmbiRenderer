#include "Material.hpp"

bool Lambertian::scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
	// 随机得到新的散射方向, 正确的漫反射
	auto scatter_direction = rec.normal + random_unit_vector();
	// 半球漫反射, 是一种不太好的早期漫反射近似, 反射与法线方向无关, 由此得到的阴影比较淡
	//auto scatter_direction = random_in_hemisphere(rec.normal);
	if (scatter_direction.near_zero()) {
		// 散射新向量很接近零向量时, 直接从法线方向散射
		scatter_direction = rec.normal;
	}
	// 返回新射线
	scattered = Ray(rec.p, scatter_direction);
	// 应用反照率
	attenuation = albedo;
	return true;
}

bool Metal::scatter(const Ray& in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
	// 镜面反射得到新的散射方向
	Vec3 reflected = reflect(unit_vector(in.direction()), rec.normal);
	// 返回新射线, 除了镜面反射外, 通过参数控制模糊程度, 给反射加上随机扰动
	scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
	// 应用反照率
	attenuation = albedo;
	return (dot(scattered.direction(), rec.normal) > 0);
}
