#include "TestApp.h"

TestApp::TestApp()
{
}

void TestApp::GetKeyHold(KeyCode key) noexcept {
	//ABR_DEBUG_OUTPUT(L"CALLBACK: GetKeyDown!" + std::to_wstring(key));
	if (key == KeyCode_K) {
		rendererManager->GetRenderer()->CleanBackBuffer();
	}
}

void TestApp::GetMouseLeftHold(float x, float y) noexcept
{
	rendererManager->GetRenderer()->SetPixel(x, y, RGB(255, 255, 255));
}

void TestApp::Update(float delta_time) noexcept
{
	AppBase::Update(delta_time);
	controlManager->Update(this);
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void TestApp::Init() noexcept{
	ABR_DEBUG_OUTPUT("TestApp Create!");
	controlManager = Singleton<ControlManager>::GetInstance();
	rendererManager = Singleton<RendererManager>::GetInstance();
}
