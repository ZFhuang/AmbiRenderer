cmake_minimum_required (VERSION 3.8)
# 项目名
project(AmbiRenderer)

# 添加子目录, 子目录要保证也有CMakeLists
add_subdirectory(RayTracer)

# 包含代码文件并设置编译目标
add_executable (${PROJECT_NAME} "AmbiRenderer.cpp" "AmbiRenderer.h")
# 连接当前Make出来的库
target_link_libraries(${PROJECT_NAME} RayTracer)