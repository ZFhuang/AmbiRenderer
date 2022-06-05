#pragma once
#include "Config.h"
#include "Utils.h"
#include <vector>

using p_frame_buffer = COLORREF*;

#define FRAME_PIXEL_IDX(x, y) y*frame_width+x

class Renderer {
public:
	Renderer() = default;
	virtual void StartUp() noexcept = 0;
	// 返回当前的帧缓冲
	virtual p_frame_buffer GetBuffer() noexcept = 0;
	// 交换内部的帧缓冲
	virtual void SwapBuffer() noexcept = 0;
};
