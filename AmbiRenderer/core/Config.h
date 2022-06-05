#pragma once
#include <string>
#include "Utils.h"
#include <Windows.h>

//namespace CONFIG
//{
//	const std::string RESOURCES = "C:/Work/AmbiRenderer/Resources/";
//}

class Config
{
public:
	std::wstring name = L"AmbiRenderer";
	std::wstring cur_renderer = L"Rasterizer";
	int screen_width = 800;
	int screen_height = 600;
	int render_width = 400;
	int render_height = 300;
	int core_num = 4;

	void StartUp() {
		ABR_DEBUG_OUTPUT("Should loading config file here...");
	}

	Config() = default;
};