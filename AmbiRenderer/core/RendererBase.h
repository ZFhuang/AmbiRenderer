#pragma once
#include "Config.h"
#include "Utils.h"
#include <vector>

using p_frame_buffer = COLORREF*;

#define FRAME_PIXEL_IDX(x, y) y*frame_width+x
#define FRAME_SIZE frame_height * frame_width

class RendererBase {
public:
	RendererBase() = default;
	virtual void StartUp() noexcept = 0;
	// 返回前缓冲
	virtual p_frame_buffer GetFrontBuffer() const noexcept;
	void SetPixel(float x, float y, COLORREF color) const noexcept;
	virtual void CopyBuffer() noexcept;
	// 交换内部的帧缓冲
	virtual void SwapBuffer() noexcept;
	// 清除后缓冲
	virtual void CleanBackBuffer() const noexcept;
	virtual void Render() noexcept = 0;
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
	p_front_buffer = new COLORREF[FRAME_SIZE];
	memset(p_front_buffer, 0, FRAME_SIZE * sizeof(COLORREF));
	p_back_buffer = new COLORREF[FRAME_SIZE];
	memset(p_back_buffer, 0, FRAME_SIZE * sizeof(COLORREF));
}

inline p_frame_buffer RendererBase::GetFrontBuffer() const noexcept {
	return p_front_buffer;
}

inline void RendererBase::SetPixel(float x, float y, COLORREF color) const noexcept
{
	GetFrontBuffer()[FRAME_PIXEL_IDX(
		static_cast<int>(x* frame_width),
		static_cast<int>(y* frame_height))] = color;
}

inline void RendererBase::CopyBuffer() noexcept
{
	memcpy(p_front_buffer, p_back_buffer, FRAME_SIZE*sizeof(COLORREF));
}

inline void RendererBase::SwapBuffer() noexcept {
	p_frame_buffer tmp = p_front_buffer;
	p_front_buffer = p_back_buffer;
	p_back_buffer = tmp;
}

inline void RendererBase::CleanBackBuffer() const noexcept {
	memset(p_back_buffer, 0, FRAME_SIZE * sizeof(COLORREF));
}

inline RendererBase::~RendererBase() noexcept
{
	delete[] p_front_buffer;
	delete[] p_back_buffer;
}
