#include "EngineCore.h"

EngineCore::EngineCore() noexcept
{
	app = new TestApp();
	gdi = Singleton<ABR_GDI>::GetInstance();
}

EngineCore::~EngineCore() noexcept
{
	delete app;
	app = nullptr;
	delete mainThread;
	mainThread = nullptr;
}

void EngineCore::StartUp() noexcept
{
	app->Create();

	ABR_DEBUG_OUTPUT("Strating thread...");
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

void EngineCore::MainThread() noexcept
{
	clock_t start = clock(), stop = clock();
	float delta_time = 0;
	
	while (true) {
		delta_time = (float)(stop - start);
		start = clock();
		app->Update(delta_time);
		Singleton<RendererManager>::GetInstance()->Draw();
		gdi->Update();

		if (thread_exited) {
			break;
		}
		stop = clock();
	}
}
