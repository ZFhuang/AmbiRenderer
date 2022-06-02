#include "WindowsRenderer.h"

// 消息处理程序
LRESULT CALLBACK HandleMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void WindowsRenderer::InitWindow() noexcept {
    WNDCLASSEX wndClass = { 0 };
    MSG msg;
    auto appName = Singleton<Config>::GetInstance()->name.c_str();
    wndClass.style = CS_HREDRAW | CS_VREDRAW;   // 设置当长宽改变时窗口重画
    wndClass.lpfnWndProc = HandleMessage;   // 消息处理回调
    wndClass.cbClsExtra = 0;    // 预留字节数
    wndClass.cbWndExtra = 0;    // 预留字节数
    wndClass.cbSize = sizeof(wndClass);
    wndClass.hInstance = root_hInstance;    // 程序本身
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // 默认图标
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // 此窗口上的指针样式
    wndClass.lpszMenuName = nullptr;    // 窗口类别菜单
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   // 白背景
    wndClass.lpszClassName = appName;   // 程序类别名称, 可以和程序名相同

    auto register_result = RegisterClassEx(&wndClass);
    assert(register_result);

    root_hwnd = CreateWindow(appName, // window class name 
        appName, // window caption 
        WS_OVERLAPPEDWINDOW, // window style 
        CW_USEDEFAULT, // initial x position 
        CW_USEDEFAULT, // initial y position 
        Singleton<Config>::GetInstance()->screen_width, // initial x size 
        Singleton<Config>::GetInstance()->screen_height, // initial y size 
        NULL, // parent window handle 
        NULL, // window menu handle 
        root_hInstance, // program instance handle 
        NULL
    );

    assert(root_hwnd != nullptr);

    ShowWindow(root_hwnd, SW_SHOWDEFAULT);  // 以默认大小显示
    UpdateWindow(root_hwnd);    // 发送WM_PAINT

    while (GetMessage(&msg, NULL, 0, 0))    // 消息循环
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK HandleMessage(HWND hwnd, UINT message, WPARAM wParam,
    LPARAM lParam)
{
    static int cxClient, cyClient, cxSource, cySource;
    HDC hdcClient, hdcWindow;
    int x, y;
    PAINTSTRUCT ps;

    // 处理消息后必须返回0
    switch (message) {
    case WM_CREATE:
        cxSource = GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXSIZE);
        cySource = GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION);
        return 0;

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;

    case WM_PAINT:  // 更新画面的消息
        hdcClient = BeginPaint(hwnd, &ps);
        hdcWindow = GetWindowDC(hwnd);

        for (y = 0; y < cyClient; y += cySource) {
            for (x = 0; x < cxClient; x += cxSource){
                BitBlt(hdcClient, x, y, cxSource, cySource, hdcWindow, 0, 0, SRCCOPY);
            }
        }

        ReleaseDC(hwnd, hdcWindow);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    // 否则返回默认的处理
    return DefWindowProc(hwnd, message, wParam, lParam);
}