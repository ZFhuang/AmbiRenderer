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
#include "../core/Renderer.h"
#include <random>

class Rasterizer :public Renderer {
public:
	Rasterizer() = default;
	void StartUp() noexcept override;
	p_frame_buffer GetBuffer() noexcept override;
	void SwapBuffer() noexcept override;
	~Rasterizer();

private:
	int frame_width;
	int frame_height;
	p_frame_buffer p_front_buffer;
	p_frame_buffer p_back_buffer;
};