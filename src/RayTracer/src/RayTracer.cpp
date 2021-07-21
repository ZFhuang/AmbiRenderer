#include <chrono>
#include <iostream>
#include "RayTracer.hpp"
#include "Sphere.hpp"
#include "MovingSphere.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include "BVH.hpp"
#include "Texture.hpp"
#include "AArect.hpp"
#include "Box.hpp"
#include "ConstantMedium.hpp"
#include "SceneManager.hpp"

bool RayTracer::render() {
	// 开始帧计时
	auto startTime = std::chrono::system_clock::now();

	// 测试输出, 保存在图片"test.ppm"中
	renderImage(Vec3(0.73, 0.73, 0.73), 10, 10);

	// 结束帧计时, 输出渲染本帧所用的毫秒数
	auto endTime = std::chrono::system_clock::now();
	std::clog << "Frame time: "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
		<< "ms" << std::endl;
	return true;
}

bool RayTracer::load_scene(SceneEnum sceneEnum)
{
	m_scene_manager = make_shared<SceneManager>(sceneEnum);
	initCamera();
	return true;
}

bool RayTracer::init_renderer(shared_ptr<Config> config)
{
	m_config = config;
	return true;
}

int RayTracer::renderImage(Color background, int sampleTimes, int max_depth) {
	int image_width = m_config->screen_width;
	int image_height = m_config->screen_height;

	// 输出流重定向到图片文件
	freopen("result.ppm", "w", stdout);
	// 头信息
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// 注意图片是从上往下, 从左往右绘制的
	for (int j = image_height - 1; j >= 0; --j) {
		std::clog << "\rProcessing: " << static_cast<double>(image_height - j) / image_height * 100 << "%;  ";
		auto line_startTime = std::chrono::system_clock::now();
		for (int i = 0; i < image_width; ++i) {
			// 计算这个像素射线计算应该着色的颜色
			Color pixel(0, 0, 0);
			// 进行超采样反走样, 也就是对每个像素都进行一点扰动然后采样多次来平均
			for (int k = 0; k < sampleTimes; ++k) {
				// 计算出当前像素映射到0-1之中的比例
				// 对每个像素索引随机扰动(0 - 1)从而让生成的射线在一个像素内有所变化, 达到超采样的效果
				double u = (static_cast<double>(i) + random_double()) / (image_width - 1);
				double v = (static_cast<double>(j) + random_double()) / (image_height - 1);
				// 利用这个比例计算对应到视平面的位置, 与相机原点相减就得到对应像素的视角射线向量
				Ray r = m_camera->getRay(u, v);
				// 累加这个像素着色的颜色
				pixel += ray_color(r, m_scene_manager->getBvhScene(), background, max_depth);
			}
			// 写入颜色到图片的像素, 内部对像素值进行了裁剪
			write_color(std::cout, pixel, sampleTimes);
		}
		// 结束行计时, 输出剩余时间
		auto line_endTime = std::chrono::system_clock::now();
		std::clog << "Scantime remaining: "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(line_endTime - line_startTime).count() / 60000.0 * j
			<< " min                  " << std::flush;
	}
	std::clog << "\nDone." << std::endl;
	return 0;
}

Color RayTracer::ray_color(const Ray& r, const HittableList& scene, const Color& background, int depth) {
	if (depth <= 0) {
		// 太远距离时返回无
		return Color(0, 0, 0);
	}

	HitRecord rec;
	// 命中球的时候会随机进行反射, 最小距离设置为0.001, 防止精度误差导致的误反射
	// 太远而没有命中返回无
	if (!scene.hit(r, 0.001, infinity, rec)) {
		return background;
	}

	// 暂存反射向量
	Ray scattered;
	// 暂存衰减值, 也就是能量被吸收的比例
	Color attenuation;
	Color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
	// 遇到发光材质时不继续递归而是返回光源颜色
	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
		// 进行递归, 每层返回都会衰减
		return emitted;
	}

	// 材质发光值+衰减值*递归值
	return emitted + attenuation * ray_color(scattered, scene, background, depth - 1);
}

int RayTracer::testImage(int image_width, int image_height) {
	// 将输出流重定向到图片文件上方便下面的写入
	freopen("test.ppm", "w", stdout);
	// 写入头信息
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// 用双重循环来写入图片信息, 要注意图片是从上往下, 从左往右绘制的, 平时我们习惯是从下往上
	for (int j = image_height - 1; j >= 0; --j) {
		// 由于标准输出流被文件占用, 因此将当前执行进度输出到日志流, 一样会显示在命令行中
		// 用\r来让输出保持在同一行
		std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			// 写入的内容暂时是测试用的渐变颜色
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.25f;
			// 由于ppm只支持整数因此需要进行类型转换
			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);
			// 写入
			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	std::clog << "\nDone." << std::endl;
	return 0;
}

void RayTracer::initCamera()
{
	m_camera = make_shared<Camera>();
	m_camera->setLookfrom(m_scene_manager->getLookfrom());
	m_camera->setLookat(m_scene_manager->getLookat());
	m_camera->setAspectRatio(m_config->aspect_ratio);
}
