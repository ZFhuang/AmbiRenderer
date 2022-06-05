#pragma once
#include "Config.h"
#include "Utils.h"
#include "../renderers/Rasterizer.h"
#include "Renderer.h"

class RendererManager {
public:
	RendererManager() noexcept;
	void StartUp() noexcept;
	void Update(HBITMAP& hdc_buffer) noexcept;

private:
	Renderer* m_renderer;
	int frame_width;
	int frame_height;
};
