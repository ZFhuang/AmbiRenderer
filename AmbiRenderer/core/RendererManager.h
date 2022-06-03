#pragma once
#include "Config.h"
#include "Utils.h"
#include "../renderers/Rasterizer.h"
#include "Renderer.h"
#include <Windows.h>

class RendererManager {
public:
	RendererManager() noexcept;
	void StartUp() noexcept;
	void Update(HDC hdc_buffer) noexcept;

private:
	Renderer* m_renderer;
};
