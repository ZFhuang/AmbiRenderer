#include "SceneManager.hpp"
#include "Scenes.hpp"
#include "BVH.hpp"

SceneManager::SceneManager()
{
	;
}

bool SceneManager::loadScene(SceneEnum sceneEnum)
{
	switch (sceneEnum)
	{
	case SceneEnum::TEST_SCENE:
		_scene = std::make_shared<TestScene>();
		break;
	case SceneEnum::RANDOM_SCENE:
		_scene = std::make_shared<RandomScene>();
		break;
	case SceneEnum::PERLIN_SCENE:
		_scene = std::make_shared<PerlinScene>();
		break;
	case SceneEnum::EARTH_SCENE:
		_scene = std::make_shared<EarthScene>();
		break;
	case SceneEnum::SIMPLE_LIGHT_SCENE:
		_scene = std::make_shared<SimpleLightScene>();
		break;
	case SceneEnum::CORNELL_ORI_SCENE:
		_scene = std::make_shared<CornelOriScene>();
		break;
	case SceneEnum::CORNELL_SMOKE_SCENE:
		_scene = std::make_shared<CornelSmokeScene>();
		break;
	case SceneEnum::FINAL_SCENE:
		_scene = std::make_shared<FinalScene>();
		break;
	default:
		throw "Not such scene!";
		break;
	}
	return true;
}

HittableList SceneManager::getRawScene()
{
	return _scene->get_HittableList();
}

HittableList SceneManager::getBvhScene()
{
	return HittableList(make_shared<BVH_Node>(_scene->get_HittableList(), 0, 1));
}
