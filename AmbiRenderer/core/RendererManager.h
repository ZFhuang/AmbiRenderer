#pragma once
#include "Config.h"
#include "Utils.h"
#include "../renderers/Rasterizer.h"
#include "RendererBase.h"

class RendererManager {
public:
	RendererManager() noexcept;
	void StartUp() noexcept;
	void Update(HBITMAP& hdc_buffer) noexcept;
	void Draw() noexcept;

private:
	RendererBase* m_renderer;
	int frame_width;
	int frame_height;
};
