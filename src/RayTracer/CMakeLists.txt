# 最低的CMake版本
cmake_minimum_required (VERSION 3.8)

project(AmbiRenderer)

# 将下列的文件加入起名的库中
add_library(RayTracer STATIC
  include/RayTracer.h
  src/RayTracer.h
  src/RayTracer.cpp
)
