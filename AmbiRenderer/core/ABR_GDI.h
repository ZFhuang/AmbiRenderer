#pragma once

class EngineCore;

#include <Windows.h>
#include <thread>
#include "Utils.h"
#include "Timer.h"
#include "Config.h"
#include "RendererManager.h"
#include "EngineCore.h"

class ABR_GDI {
public:
    ABR_GDI(HINSTANCE hInstance) noexcept : root_hInstance(hInstance) { ; }
    ~ABR_GDI();

    void Update() noexcept;

    void WaitForEnd() noexcept;

    inline void StartUp() noexcept {
        ABR_DEBUG_OUTPUT("Initing window...");

        mainThread = new std::thread([this] {MainThread(); });
        //MainThread();
    }

private:
    void MainThread() noexcept;

private:
    HINSTANCE root_hInstance;
    HWND root_hwnd = nullptr;
    std::thread* mainThread = nullptr;
    friend EngineCore;
};
