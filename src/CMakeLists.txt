cmake_minimum_required (VERSION 3.8)
# 项目名, 后面可以跟版本号
project(AmbiRenderer VERSION 0.2)

# 添加子目录, 子目录要保证也有CMakeLists
add_subdirectory(RayTracer)
add_subdirectory(ImGui)

# 包含代码文件并设置编译目标, 这里设置生成出来的可执行文件名字和工程名一样
add_executable (${PROJECT_NAME} "AmbiRenderer.cpp" "AmbiRenderer.hpp")

# 连接DX12库
target_link_libraries(${PROJECT_NAME} PRIVATE d3d12.lib dxgi.lib d3dcompiler.lib dxguid.lib)
# 连接光追渲染库
target_link_libraries(${PROJECT_NAME} PUBLIC RayTracer)
# 连接GUI库
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui)