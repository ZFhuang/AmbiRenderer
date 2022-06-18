#pragma once
#include "../core/AppBase.h"


class TestApp:public AppBase
{
public:
	TestApp();

	void ReactK();

	void Update(float delta_time) noexcept override;

	void Create() noexcept override;

	void Destroy() noexcept override {
		ABR_DEBUG_OUTPUT("TestApp Destroy!");
	}

private:

};