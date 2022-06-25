#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
//#include "tgaimage.h"

class Model {
private:
	std::vector<std::vector<int>> faces_;
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	int ntex();
	int nnorm();
	Vec3f vert(int i);
	Vec3f norm(int i);
	std::vector<int> face(int idx);
	std::vector<Vec2f> tex_;
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> norms_;
	//void initTextureCoord(TGAImage& texture);
};

#endif //__MODEL_H__
