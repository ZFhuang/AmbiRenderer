#include "RendererManager.h"

RendererManager::RendererManager() noexcept
{
	const auto& cur_renderer = Singleton<Config>::GetInstance()->cur_renderer;
	if (cur_renderer == std::wstring(L"Rasterizer")) {
		ABR_DEBUG_OUTPUT("Loading Rasterizer");
		m_renderer = new Rasterizer;
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
	p_frame_buffer frame = m_renderer->GetBuffer();

	// 用渲染器返回的帧刷新hdc
	SetBitmapBits(bitmap, frame_height * frame_width * sizeof(COLORREF), frame);
}

void RendererManager::Draw() noexcept
{
	m_renderer->Draw();
	m_renderer->SwapBuffer();
}
