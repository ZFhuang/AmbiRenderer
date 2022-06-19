#include "ABR_GDI.h"

// 消息处理程序
LRESULT CALLBACK HandleMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

ABR_GDI::~ABR_GDI()
{
	delete mainThread;
	mainThread = nullptr;
}

void ABR_GDI::MainThread() noexcept
{
	WNDCLASSEX wndClass = { 0 };

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

	MSG msg;
	ABR_DEBUG_OUTPUT("Starting thread...");
	while (GetMessage(&msg, NULL, 0, 0))    // 消息循环
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void ABR_GDI::Update() noexcept
{
	InvalidateRect(root_hwnd, NULL, false);	// 标记整个画面失效
	RedrawWindow(root_hwnd, NULL, NULL, RDW_INTERNALPAINT);    // 发送WM_PAINT进行绘制
}

void ABR_GDI::WaitForEnd() noexcept
{
	if (mainThread && mainThread->joinable()) {
		mainThread->join();
	}
	Singleton<EngineCore>::GetInstance()->Shutdown();
}

LRESULT CALLBACK HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	// static防止多次生成
	static int cxClient, cyClient;
	static HDC hdcBuffer;
	static HBITMAP hBitmap;

	// 处理消息后必须返回0
	switch (message) {
	case WM_KILLFOCUS:
		// 失焦时清除所有按键状态防止死循环
		Singleton<ControlManager>::GetInstance()->CleanState();
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		Singleton<ControlManager>::GetInstance()->OnKeyDown(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Singleton<ControlManager>::GetInstance()->OnKeyUp(static_cast<unsigned char>(wParam));
		break;

	case WM_CREATE: {
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		hdc = GetDC(hWnd);
		hdcBuffer = CreateCompatibleDC(hdc);
		hBitmap = CreateBitmap(Singleton<Config>::GetInstance()->render_width, Singleton<Config>::GetInstance()->render_height, 1, 32, NULL);
		SelectObject(hdcBuffer, hBitmap);
		ReleaseDC(hWnd, hdc);
		break;
	}

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;

	case WM_PAINT: {  // 更新画面的消息
		hdc = BeginPaint(hWnd, &ps);
		Timer timer;
		// 刷新帧画面
		Singleton<RendererManager>::GetInstance()->Update(hBitmap);
		SelectObject(hdcBuffer, hBitmap);
		// 渲染到画面
		StretchBlt(hdc,
			0,
			0,
			cxClient,
			cyClient,
			hdcBuffer,
			0,
			0,
			Singleton<Config>::GetInstance()->render_width,
			Singleton<Config>::GetInstance()->render_height,
			SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY: {
		DeleteDC(hdcBuffer);
		DeleteObject(hBitmap);

		PostQuitMessage(0);
		break;
	}

	case WM_MOUSEMOVE: {
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		//ABR_DEBUG_OUTPUT(L"Mouse Position: ("+std::to_wstring(xPos) + L", " + std::to_wstring(yPos)+L")");
		break;
	}

	default: {
		// 否则返回默认的处理
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	// 消息捕获后必须返回0
	return 0;
}