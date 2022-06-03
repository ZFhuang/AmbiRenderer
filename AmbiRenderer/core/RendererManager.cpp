#include "RendererManager.h"

RendererManager::RendererManager() noexcept
{
	const auto& cur_renderer = Singleton<Config>::GetInstance()->cur_renderer;
	if (cur_renderer == std::wstring(L"Rasterizer")) {
		ABR_DEBUG_OUTPUT("Loading Rasterizer");
		m_renderer = new Rasterizer;
	}
	else {
		// No such renderer!
		assert(false);
	}
}

void RendererManager::StartUp() noexcept
{
	m_renderer->StartUp();
}

void RendererManager::Update(HDC hdc_buffer) noexcept
{
	m_renderer->SwapBuffer();
	SetPixel(hdc_buffer, 100, 100, RGB(0, 0, 0));
	SetPixel(hdc_buffer, 101, 100, RGB(0, 0, 0));
	SetPixel(hdc_buffer, 100, 101, RGB(0, 0, 0));
	SetPixel(hdc_buffer, 101, 101, RGB(0, 0, 0));
}
