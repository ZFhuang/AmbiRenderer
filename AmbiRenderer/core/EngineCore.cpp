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
	app->Init();
	mainThread = new std::thread([this]{MainThread();});
}

void EngineCore::Shutdown() noexcept
{
	ABR_DEBUG_OUTPUT("Exiting thread...");
	thread_exited = true;
}

void EngineCore::WaitForEnd() noexcept
{
	if (mainThread && mainThread->joinable()) {
		mainThread->join();
	}
}

void EngineCore::BindFunc(KEY k, void(AppBase::* func)())
{
	(* app_event_map)[k] = func;
}

void EngineCore::ReactFunc(KEY k)
{
	auto func = (*app_event_map)[k];
	if (func != nullptr) {
		(app->*func)();
	}
}

void EngineCore::MainThread() noexcept
{
	clock_t start = clock(), stop = clock();
	float delta_time = 0;
	
	app->Create();

	while (true) {
		delta_time = (float)(stop - start);
		start = clock();

		control_manager->Update();
		app->Update(delta_time);
		renderer_manager->Draw();
		gdi->Update();

		if (thread_exited) {
			break;
		}
		stop = clock();
	}

	app->Destory();
}
