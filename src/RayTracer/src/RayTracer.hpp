#pragma once

#include "Camera.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"

using std::shared_ptr;
using std::make_shared;

// 光追渲染器的入口
int run_RayTracer();
// 将颜色数据写入图片中
int renderImage(int image_width, int image_height, Camera cam, HittableList scene, Color background, int sampleTimes, int max_depth);
// 根据输入的射线返回着色
Color ray_color(const Ray& r, const HittableList& scene, const Color& background, int depth);
// 测试输出颜色到图片
int testImage(int image_width, int image_height);
