#include "WindowsRenderer.h"

void ComponentsInitialize(HINSTANCE hInstance) {
    ABR_RUN_FUNCTION(Singleton<Config>::Initialize());
    ABR_RUN_FUNCTION(Singleton<WindowsRenderer>::Initialize(hInstance));
}

void ComponentsStartUp(void) {
    ABR_RUN_FUNCTION(Singleton<Config>::GetInstance()->StartUp());
    ABR_RUN_FUNCTION(Singleton<WindowsRenderer>::GetInstance()->StartUp());
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
) {
    ComponentsInitialize(hInstance);
    ComponentsStartUp();
    return 0;
}