#pragma once
//#include <utility>
//#include <cstdlib>
//#include <string>
//#include <limits>
//#include <vector>
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
#include "../core/Renderer.h"

class Rasterizer :public Renderer {
public:
	Rasterizer() noexcept;
	void StartUp() noexcept override;
	void SwapBuffer() noexcept override;

private:
	int width;
	int height;
};