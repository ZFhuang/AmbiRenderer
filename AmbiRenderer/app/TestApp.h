#pragma once
#include "../core/AppBase.h"

class TestApp:public AppBase
{
public:
	TestApp();

	void Update(float delta_time) noexcept override {
		AppBase::Update(delta_time);
	}

	void Create() noexcept override {
		ABR_DEBUG_OUTPUT("TestApp Create!");
	}

	void Destory() noexcept override {
		ABR_DEBUG_OUTPUT("TestApp Destory!");
	}

private:

};