#pragma once

#include <iostream>
#include "Vec3.hpp"

// 光追渲染器的入口
int run_RayTracer();
// 设置相机参数
Vec3* setCamera(double viewport_width, double viewport_height, double focal_length);
// 将颜色数据写入图片中
int renderImage(int image_width, int image_height, Vec3 camera_args[4]);
// 测试输出颜色到图片
int testImage(int image_width, int image_height);
