#pragma once
#include <string>

namespace PATH
{
	const std::string RESOURCES = "C:/Work/AmbiRenderer/src/Resources/";
}

class Config {
public:
	Config(int width = 400, int height = 400) :screen_width(width), screen_height(height) {
		aspect_ratio = screen_width / screen_height;
	}
	int screen_width;
	int screen_height;
	double aspect_ratio;
};
