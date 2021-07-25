#include "AmbiRenderer.hpp"
#include <iostream>
#include "Renderer.hpp"
#include "../../RayTracer/core/source/RayTracer.hpp"
#include "../../RayTracer/core/source/SceneManager.hpp"
#include <memory>

using std::make_shared;
using std::shared_ptr;
class Config;

int main(int argc, char* argv[])
{
	std::cout << "Start..." << std::endl;

	shared_ptr<BaseRenderer> renderer = make_shared<RayTracer>();
	shared_ptr<Config> config = make_shared<Config>();

	renderer->init_renderer(config);
	renderer->load_scene(SceneEnum::FINAL_SCENE);

	renderer->render();

	std::cout << "Exit!" << std::endl;
	return 0;
}
