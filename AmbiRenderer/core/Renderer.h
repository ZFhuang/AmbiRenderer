#pragma once
#include "Config.h"
#include "Utils.h"

class Renderer {
public:
	Renderer() = default;
	virtual void StartUp() noexcept = 0;
	virtual void SwapBuffer() noexcept = 0;
};
