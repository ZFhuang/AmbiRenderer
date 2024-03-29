#pragma once

class ABR_GDI;
class AppBase;
class ControlManager;

#include <thread>
#include <vector>
#include "Config.h"
#include "ABR_GDI.h"
#include "Utils.h"
#include "AppBase.h"
#include "RendererBase.h"
#include "ControlManager.h"
#include "Timer.h"

class EngineCore {
	using AppEventMap = std::vector<void (AppBase::*)()>;

public:
	EngineCore(AppBase* target_app) noexcept;
	~EngineCore() noexcept;
	void StartUp() noexcept;
	void Shutdown() noexcept;
	void WaitForEnd() noexcept;

private:
	void MainThread() noexcept;
	void GameplayTick(float delta_time) noexcept;
	void RenderTick(float delta_time) noexcept;

private:
	AppBase* app = nullptr;
	ABR_GDI* gdi = nullptr;
	RendererManager* renderer_manager = nullptr;
	ControlManager* control_manager = nullptr;
	bool thread_exited = false;
	std::thread* mainThread = nullptr;
	AppEventMap* app_event_map = nullptr;
};
