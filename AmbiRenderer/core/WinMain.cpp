#include "ABR_GDI.h"
#include <thread>

static std::atomic<bool> gdi_exited = false;

void ComponentsInitialize(HINSTANCE hInstance) {
    ABR_RUN_FUNCTION(Singleton<Config>::Initialize());
    ABR_RUN_FUNCTION(Singleton<RendererManager>::Initialize());
    ABR_RUN_FUNCTION(Singleton<ABR_GDI>::Initialize(hInstance));
}

std::thread ComponentsStartUp(void) {
    ABR_RUN_FUNCTION(Singleton<Config>::GetInstance()->StartUp());
    ABR_RUN_FUNCTION(Singleton<RendererManager>::GetInstance()->StartUp());
    
    std::thread t_GDI = std::thread([]() {
        ABR_RUN_FUNCTION(Singleton<ABR_GDI>::GetInstance()->StartUp());
        gdi_exited = true;
        });

    return t_GDI;
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
) {
    ComponentsInitialize(hInstance);
    std::thread t_GDI = ComponentsStartUp();

    while (true) {
        Sleep(30);
        Singleton<ABR_GDI>::GetInstance()->Update();
        if (gdi_exited) {
            break;
        }
    }

    t_GDI.join();

    return 0;
}