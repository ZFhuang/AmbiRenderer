#pragma once
#include "Config.h"
#include "Utils.h"
#include <vector>

using p_frame_buffer = COLORREF*;

#define FRAME_PIXEL_IDX(x, y) y*frame_width+x

class RendererBase {
public:
	RendererBase() = default;
	virtual void StartUp() noexcept = 0;
	// 返回前缓冲
	virtual p_frame_buffer GetFrontBuffer() noexcept;
	// 交换内部的帧缓冲
	virtual void SwapBuffer() noexcept;
	// 清除后缓冲
	virtual void CleanBackBuffer() noexcept;
	virtual void Draw() noexcept = 0;
	virtual ~RendererBase() noexcept = 0;

protected:
	int frame_width = 0;
	int frame_height = 0;
	p_frame_buffer p_front_buffer = nullptr;
	p_frame_buffer p_back_buffer = nullptr;
};

inline void RendererBase::StartUp() noexcept {
	frame_height = Singleton<Config>::GetInstance()->render_height;
	frame_width = Singleton<Config>::GetInstance()->render_width;
	p_front_buffer = new COLORREF[frame_height * frame_width];
	p_back_buffer = new COLORREF[frame_height * frame_width];
}

inline p_frame_buffer RendererBase::GetFrontBuffer() noexcept {
	return p_front_buffer;
}

inline void RendererBase::SwapBuffer() noexcept {
	p_frame_buffer tmp = p_front_buffer;
	p_front_buffer = p_back_buffer;
	p_back_buffer = tmp;
	CleanBackBuffer();
}

inline void RendererBase::CleanBackBuffer() noexcept {
	static int sum_elem = frame_height * frame_width;
	for (int idx = 0; idx < sum_elem; ++idx) {
		p_back_buffer[idx] = RGB(0, 0, 0);
	}
}

inline RendererBase::~RendererBase() noexcept
{
	delete[] p_front_buffer;
	delete[] p_back_buffer;
}
