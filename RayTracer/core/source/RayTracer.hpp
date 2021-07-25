#pragma once

#include "Camera.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
#include "../../../AmbiRenderer/core/Renderer.hpp"
#include "../include/Config.hpp"

using std::shared_ptr;
using std::make_shared;

enum class SceneEnum;
class SceneManager;

class RayTracer : public BaseRenderer {
public:
	virtual bool init_renderer(shared_ptr<Config> config) override;
	virtual bool render() override;
	virtual bool load_scene(SceneEnum sceneEnum) override;

private:
	// 将颜色数据写入图片中
	int renderImage(Color background, int sampleTimes, int max_depth);
	// 根据输入的射线返回着色
	Color ray_color(const Ray& r, const HittableList& scene, const Color& background, int depth);
	// 测试输出颜色到图片
	int testImage(int image_width, int image_height);
	void initCamera();

private:
	shared_ptr<Config> m_config;
	shared_ptr<Camera> m_camera;
	shared_ptr<SceneManager> m_scene_manager;
};
