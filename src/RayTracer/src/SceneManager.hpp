#pragma once

#include "RayTracer.hpp"
#include "Scenes.hpp"
#include <memory>

enum class SceneEnum
{
	TEST_SCENE = 1,
	RANDOM_SCENE=2,
	PERLIN_SCENE=3,
	EARTH_SCENE=4,
	SIMPLE_LIGHT_SCENE=5,
	CORNELL_ORI_SCENE=6,
	CORNELL_SMOKE_SCENE=7,
	FINAL_SCENE=8
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
