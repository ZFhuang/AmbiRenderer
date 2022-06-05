#pragma once
#include <Windows.h>
#include "Utils.h"
#include "Config.h"
#include "RendererManager.h"

class ABR_GDI {
public:
    ABR_GDI(HINSTANCE hInstance) noexcept : root_hInstance(hInstance) { ; }

    void InitWindow() noexcept;

    void StartHandleMessage() noexcept;

    void Update() noexcept;

    void KeyBoardMessage(WPARAM wParam) noexcept;

    inline void StartUp() noexcept {
        ABR_DEBUG_OUTPUT("Starting...");
        InitWindow();
        StartHandleMessage();
    }

private:
    HINSTANCE root_hInstance;
    HWND root_hwnd = nullptr;
};
