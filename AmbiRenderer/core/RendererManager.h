#pragma once
#include <thread>
#include "Config.h"
#include "Utils.h"
#include "../renderers/rasterizer/Rasterizer.h"
#include "../renderers/painter/Painter.h"
#include "RendererBase.h"
#include "Timer.h"

class RendererManager {
public:
	RendererManager() noexcept;
	void StartUp() noexcept;
	void Update(HBITMAP& hdc_buffer) noexcept;
	void MainThread() noexcept;
	void ClockThread() noexcept;
	void Shutdown() noexcept;
	const RendererBase* GetRenderer() noexcept;

private:
	RendererBase* m_renderer = nullptr;
	std::thread* mainThread = nullptr;
	std::thread* clockThread = nullptr;
	bool thread_exited = false;
	int frame_width = 0;
	int frame_height = 0;
	int target_framerate = 0;
};
