#include "RayTracer.hpp"
#include "SceneManager.hpp"
#include "../include/Config.hpp"
#include <memory>
#include <iostream>

using std::shared_ptr;
using std::make_shared;

int startUp() {
	shared_ptr<RayTracer> renderer = make_shared<RayTracer>();
	shared_ptr<Config> config = make_shared<Config>();

	renderer->init_renderer(config);
	renderer->load_scene(SceneEnum::FINAL_SCENE);

	renderer->render();

	return 0;
}

int main(int argc, char* argv[]) {
	std::cout << "Start..." << std::endl;

	startUp();

	std::cout << "Exit!" << std::endl;
	return 0;
}
