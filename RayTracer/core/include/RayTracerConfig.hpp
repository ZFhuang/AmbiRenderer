#pragma once
#include <string>
#include "../../../AmbiRenderer/core/Config.hpp"

namespace PATH
{
	const std::string RESOURCES = "C:/Work/AmbiRenderer/Resources/";
}

class RayTracerConfig : public Config {
public:
	RayTracerConfig(int width = 400, int height = 400) :screen_width(width), screen_height(height) {
		aspect_ratio = screen_width / screen_height;
	}
	int screen_width;
	int screen_height;
	double aspect_ratio;
};
