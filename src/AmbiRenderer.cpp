#include "AmbiRenderer.hpp"

int main(int argc, char* argv[])
{
	std::cout << "Start..." << std::endl;

	shared_ptr<BaseRenderer> renderer = make_shared<RayTracer>();
	shared_ptr<Config> config = make_shared<Config>();

	renderer->init_renderer(config);
	renderer->load_scene(SceneEnum::TEST_SCENE);
	renderer->render();

	std::cout << "Exit!" << std::endl;
	return 0;
}
