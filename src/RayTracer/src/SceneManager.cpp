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
