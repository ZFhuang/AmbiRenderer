#include "TestApp.h"
#include <functional>

TestApp::TestApp()
{
}

void TestApp::ReactK()
{
	ABR_DEBUG_OUTPUT("K Down!");
}

void TestApp::Update(float delta_time) noexcept
{
	AppBase::Update(delta_time);
	Sleep(10);
}

void TestApp::Create() noexcept{
	ABR_DEBUG_OUTPUT("TestApp Create!");
	AppBase::BindKeyDown(KB_K, static_cast<pFunc>(&TestApp::ReactK));
}
