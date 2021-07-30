#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "../include/geometry.h"

class Model {
private:
	std::vector<std::vector<int>> faces_;
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	int ntex();
	Vec3f vert(int i);
	std::vector<int> face(int idx);
	std::vector<Vec2f> tex_;
	std::vector<Vec3f> verts_;
};

#endif //__MODEL_H__
