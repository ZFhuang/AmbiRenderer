#pragma once

#ifdef DLL_EXPORT
#define DLL_API __declspect( dllexport )
#else
#define DLL_API __declspect( dllimport )
#endif // DLL_EXPORT


// 用来给外部包含, 作为一个跳转
#include "../src/RayTracer.hpp"