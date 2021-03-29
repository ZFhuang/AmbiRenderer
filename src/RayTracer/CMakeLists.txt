# 将下列的文件加入起名的库中
set(target_name "RayTracer")

set(RAYTRACER_FILES
	 "include/RayTracer.hpp"
	 "src/RayTracer.hpp"
	 "src/RayTracer.cpp" 
	 "include/Vec3.hpp" 
	 "src/Vec3.hpp"
	 "src/Color.hpp"
	 "include/Color.hpp" 
	 "include/Ray.hpp" 
	 "src/Ray.hpp"
	 "src/Hittable.hpp"
	 "include/Hittable.hpp"
	 "src/Sphere.hpp"
	 "include/Sphere.hpp"
	 "src/HittableList.hpp"
	 "include/HittableList.hpp"
	 "src/RTweekend.hpp"
	  "include/RTweekend.hpp" "src/Camera.hpp" "include/Camera.hpp" "src/HittableList.cpp" "src/Sphere.cpp" "src/Material.hpp" "src/Material.cpp" "include/Material.hpp" "src/MovingSphere.hpp" "src/MovingShpere.cpp" "src/AABB.hpp" "src/BVH.hpp" "src/BVH.cpp" "src/AABB.cpp" "src/Texture.hpp" "src/Perlin.hpp")

# 目前不知道为什么动态链接不能生产ilk文件, 所以使用了静态链接
# 将上面的文件添加到库${target_name}中, 也就是RayTracer
add_library(${target_name} STATIC ${RAYTRACER_FILES})

# 设置目录和编译模式
target_include_directories(${target_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${target_name} PRIVATE DLL_EXPORT)