# 将下列的文件加入起名的库中
set(target_name "RayTracer")

set(RAYTRACER_FILES
	include/RayTracer.hpp
	src/RayTracer.hpp
	src/RayTracer.cpp)

add_library(${target_name} STATIC ${RAYTRACER_FILES})

target_include_directories(${target_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${target_name} PRIVATE DLL_EXPORT)