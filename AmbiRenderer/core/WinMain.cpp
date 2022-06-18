#include <thread>

#include "Systems.h"
#include "../app/TestApp.h"

void SystemsInitialize(HINSTANCE hInstance) {
    ABR_STATE_FUNCTION("INITING", Singleton<Config>::Initialize());
    ABR_STATE_FUNCTION("INITING", Singleton<ControlManager>::Initialize());
    ABR_STATE_FUNCTION("INITING", Singleton<RendererManager>::Initialize());
    ABR_STATE_FUNCTION("INITING", Singleton<ABR_GDI>::Initialize(hInstance));
    AppBase* app = new TestApp();
    ABR_STATE_FUNCTION("INITING", Singleton<EngineCore>::Initialize(app));
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

void SystemsDestroy(void) {
    ABR_STATE_FUNCTION("QUITING", Singleton<RendererManager>::Destroy());
    ABR_STATE_FUNCTION("QUITING", Singleton<ABR_GDI>::Destroy());
    ABR_STATE_FUNCTION("QUITING", Singleton<EngineCore>::Destroy());
    ABR_STATE_FUNCTION("QUITING", Singleton<ControlManager>::Destroy());
    ABR_STATE_FUNCTION("QUITING", Singleton<Config>::Destroy());
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
    SystemsDestroy();
    return 0;
}