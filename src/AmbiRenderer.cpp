#include "AmbiRenderer.hpp"

int main(int argc, char* argv[])
{
	std::cout << "Start..." << std::endl;
	// 测试GUI
	imgui_example(argc, argv);
	// 调用光追渲染器
	//run_RayTracer();
	std::cout << "Exit!" << std::endl;
	return 0;
}
