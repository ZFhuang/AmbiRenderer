#pragma once
#include "Config.h"
#include "Utils.h"
#include "../renderers/rasterizer/Rasterizer.h"
#include "../renderers/painter/Painter.h"
#include "RendererBase.h"

class RendererManager {
public:
	RendererManager() noexcept;
	void StartUp() noexcept;
	void Update(HBITMAP& hdc_buffer) noexcept;
	void Draw() noexcept;

private:
	RendererBase* m_renderer = nullptr;
	int frame_width = 0;
	int frame_height = 0;
};
