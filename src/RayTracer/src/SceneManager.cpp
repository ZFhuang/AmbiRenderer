#include "SceneManager.hpp"
#include "Scenes.hpp"
#include "BVH.hpp"
#include <cassert>

SceneManager::SceneManager()
{
	;
}

SceneManager::SceneManager(SceneEnum sceneEnum)
{
	loadScene(sceneEnum);
}

bool SceneManager::loadScene(SceneEnum sceneEnum)
{
	switch (sceneEnum)
	{
	case SceneEnum::TEST_SCENE:
		_scene = std::make_shared<TestScene>();
		_default_lookfrom = Point3(13, 2, 3);
		_default_lookat = Point3(0, 0, 0);
		break;
	case SceneEnum::RANDOM_SCENE:
		_scene = std::make_shared<RandomScene>();
		_default_lookfrom = Point3(13, 2, 3);
		_default_lookat = Point3(0, 0, 0);
		break;
	case SceneEnum::PERLIN_SCENE:
		_scene = std::make_shared<PerlinScene>();
		_default_lookfrom = Point3(13, 2, 3);
		_default_lookat = Point3(0, 0, 0);
		break;
	case SceneEnum::EARTH_SCENE:
		_scene = std::make_shared<EarthScene>();
		_default_lookfrom = Point3(13, 2, 3);
		_default_lookat = Point3(0, 0, 0);
		break;
	case SceneEnum::SIMPLE_LIGHT_SCENE:
		_scene = std::make_shared<SimpleLightScene>();
		_default_lookfrom = Point3(26, 3, 6);
		_default_lookat = Point3(0, 2, 0);
		break;
	case SceneEnum::CORNELL_ORI_SCENE:
		_scene = std::make_shared<CornelOriScene>();
		_default_lookfrom = Point3(278, 278, -800);
		_default_lookat = Point3(278, 278, 0);
		break;
	case SceneEnum::CORNELL_SMOKE_SCENE:
		_scene = std::make_shared<CornelSmokeScene>();
		_default_lookfrom = Point3(278, 278, -800);
		_default_lookat = Point3(278, 278, 0);
		break;
	case SceneEnum::FINAL_SCENE:
		_scene = std::make_shared<FinalScene>();
		_default_lookfrom = Point3(478, 278, -600);
		_default_lookat = Point3(278, 278, 0);
		break;
	default:
		assert(true&&"Load undefined scene");
		break;
	}
	return true;
}

HittableList SceneManager::getRawScene()
{
	assert(_scene != nullptr && "Get scene before loaded");
	return _scene->get_HittableList();
}

HittableList SceneManager::getBvhScene()
{
	assert(_scene != nullptr && "Get scene before loaded");
	return HittableList(make_shared<BVH_Node>(_scene->get_HittableList(), 0, 1));
}

Point3 SceneManager::getLookfrom()
{
	assert(_scene != nullptr&&"Get cam 'lookfrom' before scene load");
	return _default_lookfrom;
}

Point3 SceneManager::getLookat()
{
	assert(_scene != nullptr && "Get cam 'lookat' before scene load");
	return _default_lookat;
}
