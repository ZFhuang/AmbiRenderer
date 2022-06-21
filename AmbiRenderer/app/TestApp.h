#pragma once
#include "../core/AppBase.h"
#include "../core/ControlManager.h"

class TestApp : public AppBase
{
public:
	TestApp();

	void GetKeyDown(KeyCode key) noexcept;
	void GetKeyUp(KeyCode key) noexcept;
	void GetMouseLeftHold(int x, int y) noexcept;

	void Update(float delta_time) noexcept override;

	void Init() noexcept override;

	void Destroy() noexcept override {
		ABR_DEBUG_OUTPUT("TestApp Destroy!");
	}

private:
	ControlManager* controlManager = nullptr;
};