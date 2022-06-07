#include "Systems.h"
#include <thread>

static std::atomic<bool> gdi_exited = false;

void SystemsInitialize(HINSTANCE hInstance) {
    ABR_STATE_FUNCTION("INITING", Singleton<Config>::Initialize());
    ABR_STATE_FUNCTION("INITING", Singleton<RendererManager>::Initialize());
    ABR_STATE_FUNCTION("INITING", Singleton<ABR_GDI>::Initialize(hInstance));
    ABR_STATE_FUNCTION("INITING", Singleton<EngineCore>::Initialize());
}

void SystemsStartUp(void) {
    ABR_STATE_FUNCTION("RUNNING", Singleton<RendererManager>::GetInstance()->StartUp());
    ABR_STATE_FUNCTION("RUNNING", Singleton<ABR_GDI>::GetInstance()->StartUp());
    ABR_STATE_FUNCTION("RUNNING", Singleton<EngineCore>::GetInstance()->StartUp());
}

void SystemsWaitForEnd(void) {
    ABR_STATE_FUNCTION("WAITING", Singleton<ABR_GDI>::GetInstance()->WaitForEnd());
    ABR_STATE_FUNCTION("WAITING", Singleton<EngineCore>::GetInstance()->WaitForEnd());
}

void SystemsQuit(void) {
    ABR_STATE_FUNCTION("QUITING", Singleton<RendererManager>::Destory());
    ABR_STATE_FUNCTION("QUITING", Singleton<ABR_GDI>::Destory());
    ABR_STATE_FUNCTION("QUITING", Singleton<EngineCore>::Destory());
    ABR_STATE_FUNCTION("QUITING", Singleton<Config>::Destory());
}

int WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
    SystemsInitialize(hInstance);
    SystemsStartUp();
    SystemsWaitForEnd();
    SystemsQuit();
    return 0;
}