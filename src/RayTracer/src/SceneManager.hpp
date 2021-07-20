#pragma once

#include "RayTracer.hpp"
#include "Scenes.hpp"
#include <memory>

enum class SceneEnum
{
	TEST_SCENE = 1
};

class SceneManager {
public:
	SceneManager();
	bool loadScene(SceneEnum sceneEnum);
	HittableList getRawScene();
	HittableList getBvhScene();

private:
	std::shared_ptr<Scene> _scene;
};
