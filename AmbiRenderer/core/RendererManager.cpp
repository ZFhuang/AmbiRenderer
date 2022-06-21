#include "RendererManager.h"

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
	target_framerate = Singleton<Config>::GetInstance()->target_framerate;

	mainThread = new std::thread([this] {MainThread(); });
	clockThread = new std::thread([this] {ClockThread(); });
}

void RendererManager::Update(HBITMAP& bitmap) noexcept
{
	p_frame_buffer frame = m_renderer->GetFrontBuffer();

	// 用渲染器返回的帧刷新hdc
	SetBitmapBits(bitmap, frame_height * frame_width * sizeof(COLORREF), frame);
}

void RendererManager::MainThread() noexcept
{
	Timer render_timer;
	int64_t delta_time = 0;
	while (!thread_exited) {
		delta_time = render_timer.elapsed_milliseconds();
		render_timer.reset();
		//ABR_DEBUG_OUTPUT(L"Render used: "+std::to_wstring(delta_time)+ L"ms");
		m_renderer->Draw();
		m_renderer->SwapBuffer();
		//m_renderer->CleanBackBuffer();
	}
}

void RendererManager::ClockThread() noexcept
{
	while (!thread_exited) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / target_framerate));
		// force refresh
		m_renderer->CopyBuffer();
	}
}

const RendererBase* RendererManager::GetRenderer() noexcept
{
	return m_renderer;
}

void RendererManager::Shutdown() noexcept
{
	ABR_DEBUG_OUTPUT("Exiting thread...");
	thread_exited = true;
}
