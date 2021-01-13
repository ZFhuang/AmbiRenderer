#pragma once

#include <iostream>
#include <memory>
#include <chrono>

#include "RayTracer.hpp"
#include "Color.hpp"
#include "Sphere.hpp"
#include "RTweekend.hpp"
#include "Camera.hpp"
#include "HittableList.hpp"

using std::shared_ptr;
using std::make_shared;

// 光追渲染器的入口
int run_RayTracer();
// 将颜色数据写入图片中
int renderImage(int image_width, int image_height, Camera cam, HittableList scene, int sampleTimes);
// 根据输入的射线返回着色
Color ray_color(const Ray& r, const HittableList& scene);
// 测试输出颜色到图片
int testImage(int image_width, int image_height);
