#include "Gui_core.hpp"
#include "lib/imgui.h"

// Our state
bool show_demo_window = false;
bool show_metrics_window = true;

void gui_core() {
	if (show_demo_window)
		ShowDemoWindow();
	if(show_metrics_window)
		ShowMetricsWindow();
	//ShowMainWindow();
}

void ShowDemoWindow() {
	ImGui::SetNextWindowPos(ImVec2(0, 250));
	ImGui::SetNextWindowSize(ImVec2(350, 500));
	ImGui::ShowDemoWindow();
}

void ShowMetricsWindow() {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(350, 250));
	ImGui::ShowMetricsWindow();
}

void ShowMainWindow() {
	//// 绘制新窗口
	//ImGui::Begin("Hello, world!");

	//ImGui::SetWindowPos(ImVec2(350, 0));
	//ImGui::SetWindowSize(ImVec2(900, 750));

	//// 结束当前窗口绘制
	//ImGui::End();
}