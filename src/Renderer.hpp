#pragma once

#include <memory>
#include "Config.hpp"
#include "RayTracer/src/SceneManager.hpp"

using std::shared_ptr;
using std::make_shared;

class BaseRenderer {
public:
	virtual bool init_renderer(shared_ptr<Config> config) = 0;
	virtual bool render() = 0;
	virtual bool load_scene(SceneEnum sceneEnum) = 0;
};
