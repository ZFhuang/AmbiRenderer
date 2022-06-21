#pragma once
#include "../core/AppBase.h"
#include "../core/ControlManager.h"
#include "../core/RendererManager.h"

class TestApp : public AppBase
{
public:
	TestApp();

	void GetKeyHold(KeyCode key) noexcept;
	void GetMouseLeftHold(float x, float y) noexcept;

	void Update(float delta_time) noexcept override;

	void Init() noexcept override;

	void Destroy() noexcept override {
		ABR_DEBUG_OUTPUT("TestApp Destroy!");
	}

private:
	ControlManager* controlManager = nullptr;
	RendererManager* rendererManager = nullptr;
};