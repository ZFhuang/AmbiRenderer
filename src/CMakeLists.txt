cmake_minimum_required (VERSION 3.8)
# 项目名
project(AmbiRenderer VERSION 0.1)

# 添加子目录, 子目录要保证也有CMakeLists
add_subdirectory(RayTracer)

# 包含代码文件并设置编译目标, 这里设置生成出来的可执行文件名字和工程名一样
add_executable (${PROJECT_NAME} "AmbiRenderer.cpp" "AmbiRenderer.hpp")
# 连接当前Make出来的库
target_link_libraries(${PROJECT_NAME} PUBLIC RayTracer)