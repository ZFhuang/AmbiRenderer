#pragma once
#include <Windows.h>
#include "Utils.h"
#include "Config.h"

class WindowsRenderer {
public:
    WindowsRenderer(HINSTANCE hInstance) noexcept : root_hInstance(hInstance) { ; }

    void InitWindow() noexcept;

    inline void StartUp() noexcept {
        ABR_DEBUG_OUTPUT("Starting...");
        InitWindow();
    }

private:
    HINSTANCE root_hInstance;
    HWND root_hwnd = nullptr;
};
