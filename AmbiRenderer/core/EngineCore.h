#pragma once

class ABR_GDI;

#include <thread>
#include "Config.h"
#include "ABR_GDI.h"
#include "Utils.h"
#include "AppBase.h"
#include "../app/TestApp.h"
#include "RendererBase.h"

class EngineCore {
public:
	EngineCore() noexcept;
	~EngineCore() noexcept;
	void StartUp() noexcept;
	void Shutdown() noexcept;
	void WaitForEnd() noexcept;

private:
	void MainThread() noexcept;

private:
	AppBase* app = nullptr;
	ABR_GDI* gdi = nullptr;
	bool thread_exited = false;
	std::thread* mainThread = nullptr;
};
