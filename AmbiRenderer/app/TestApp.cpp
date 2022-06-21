#include "TestApp.h"

TestApp::TestApp()
{
}

void TestApp::GetKeyDown(KeyCode key) noexcept {
	ABR_DEBUG_OUTPUT(L"CALLBACK: GetKeyDown!" + std::to_wstring(key));
}

void TestApp::GetKeyUp(KeyCode key) noexcept
{
	ABR_DEBUG_OUTPUT(L"CALLBACK: GetKeyUp!" + std::to_wstring(key));
}

void TestApp::GetMouseLeftHold(int x, int y) noexcept
{
	ABR_DEBUG_OUTPUT(L"CALLBACK: GetMouseHold!" + std::to_wstring(x) +L"," + std::to_wstring(y));
}

void TestApp::Update(float delta_time) noexcept
{
	AppBase::Update(delta_time);
	controlManager->Update(this);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void TestApp::Init() noexcept{
	ABR_DEBUG_OUTPUT("TestApp Create!");
	controlManager = Singleton<ControlManager>::GetInstance();
}
