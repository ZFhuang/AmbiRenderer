#pragma once

#include "geometry.h"
#include "tgaimage.h"
#include <vector>

extern Matrix modelViewMat;
extern Matrix projMat;
extern Matrix viewportMat;

extern int width;
extern int height;
extern int depth;

class v_Shader {
public:
	virtual ~v_Shader() {};
	virtual std::vector<float> vertex(int f_idx, int v_idx) = 0;
};

class f_Shader {
public:
	virtual ~f_Shader() {};
	virtual std::vector<float> fragment(std::vector<float> f_in) = 0;
};

//void triangle(Vec3f* pts, int** zbuffer, TGAImage& image, f_Shader* f_shader);

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

Matrix makeModelRotationMat(Vec3f xyz);

Matrix makeModelTranslationMat(Vec3f xyz);

Matrix makeModelScalingMat(Vec3f xyz);

Matrix makeProjMat(float coeff);

Matrix makeViewMat(Vec3f eye, Vec3f center, Vec3f up);

Matrix makeViewportMat(int x, int y, int w, int h);

int** makeZBuffer(int height, int width);

void deleteZBuffer(int** zbuffer, int height);
