#include "EngineCore.h"

EngineCore::EngineCore(AppBase* target_app) noexcept
{
	app = target_app;
	gdi = Singleton<ABR_GDI>::GetInstance();
	renderer_manager = Singleton<RendererManager>::GetInstance();
	control_manager = Singleton<ControlManager>::GetInstance();
	app_event_map = new AppEventMap(MAXKEY, nullptr);
}

EngineCore::~EngineCore() noexcept
{
	delete app;
	app = nullptr;
	delete mainThread;
	mainThread = nullptr;
	delete app_event_map;
	app_event_map = nullptr;
}

void EngineCore::StartUp() noexcept
{
	ABR_DEBUG_OUTPUT("Strating thread...");
	app->PreInit();
	mainThread = new std::thread([this]{MainThread();});
}

void EngineCore::Shutdown() noexcept
{
	ABR_DEBUG_OUTPUT("Exiting thread...");
	thread_exited = true;
	Singleton<RendererManager>::GetInstance()->Shutdown();
}

void EngineCore::WaitForEnd() noexcept
{
	if (mainThread && mainThread->joinable()) {
		mainThread->join();
	}
}

void EngineCore::MainThread() noexcept
{
	Timer core_timer;
	float delta_time = 0;
	
	app->Init();

	while (!thread_exited) {
		delta_time = core_timer.elapsed_milliseconds() / MS_PER_SEC;
		core_timer.reset();

		GameplayTick(delta_time);
		RenderTick(delta_time);
	}

	app->Destroy();
}

void EngineCore::GameplayTick(float delta_time) noexcept
{
	app->Update(delta_time);
}

void EngineCore::RenderTick(float delta_time) noexcept
{
	gdi->Update();
	// 在标题栏显示帧数
	SetWindowText(gdi->root_hwnd, (L"AmbiRenderer (fps=" + std::to_wstring(min(1000, 1 / delta_time)) + L")").c_str());
}
