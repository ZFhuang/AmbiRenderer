﻿#include "RendererManager.h"

RendererManager::RendererManager() noexcept
{
	const auto& cur_renderer = Singleton<Config>::GetInstance()->cur_renderer;
	if (cur_renderer == std::wstring(L"Rasterizer")) {
		ABR_DEBUG_OUTPUT("Loading Rasterizer");
		m_renderer = new Rasterizer;
	}
	else if (cur_renderer == std::wstring(L"Painter")) {
		ABR_DEBUG_OUTPUT("Loading Painter");
		m_renderer = new Painter;
	}
	else {
		assert(false); // No such renderer!
	}
}

void RendererManager::StartUp() noexcept
{
	m_renderer->StartUp();
	frame_height = Singleton<Config>::GetInstance()->render_height;
	frame_width = Singleton<Config>::GetInstance()->render_width;
}

void RendererManager::Update(HBITMAP& bitmap) noexcept
{
	p_frame_buffer frame = m_renderer->GetFrontBuffer();

	// 用渲染器返回的帧刷新hdc
	SetBitmapBits(bitmap, frame_height * frame_width * sizeof(COLORREF), frame);
}

void RendererManager::Draw() noexcept
{
	m_renderer->Draw();
	m_renderer->SwapBuffer();
}

const RendererBase* RendererManager::GetRenderer() noexcept
{
	return m_renderer;
}
