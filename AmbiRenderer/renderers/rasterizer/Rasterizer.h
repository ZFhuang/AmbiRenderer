#pragma once
//#include <utility>
//#include <cstdlib>
//#include <string>
//#include <limits>
//#include <cmath>
//
//#include "tgaimage.h"
//#include "model.h"
//#include "gl_soft.h"
//#include "../shaders/v_Basic.cpp"
//#include "../shaders/f_Phong.cpp"
//#include "../shaders/f_Gouraud.cpp"
//#include "../shaders/f_Stylized.cpp"
//#include "../shaders/f_Depth.cpp"
//#include "../shaders/f_Normal.cpp"
//#include "../shaders/f_AO.cpp"
//#include "../shaders/f_SSAO.cpp"

#include <string>
#include <memory>
#include "../../core/RendererBase.h"

class Rasterizer :public RendererBase {
public:
	Rasterizer() = default;
	void StartUp() noexcept override;
	void Draw() noexcept override;
	~Rasterizer();
};