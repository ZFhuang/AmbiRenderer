#pragma once

#include "geometry.h"
#include "tgaimage.h"

extern Matrix modelViewMat;
extern Matrix projMat;
extern Matrix viewportMat;

extern int width;
extern int height;
extern int depth;

class VShader {
	virtual ~VShader();
	virtual Vec3i vertex(int iface, int nthvert) = 0;
};

class FShader {
	virtual ~FShader();
	virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
};

void triangle(Vec3f* pts, int** zbuffer, TGAImage& image, FShader& f_shader);

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

Matrix makeModelRotationMat(Vec3f xyz);

Matrix makeModelTranslationMat(Vec3f xyz);

Matrix makeModelScalingMat(Vec3f xyz);

Matrix makeProjMat(float coeff);

Matrix makeViewMat(Vec3f eye, Vec3f center, Vec3f up);

Matrix makeViewportMat(int x, int y, int w, int h);

int** makeZBuffer();

void deleteZBuffer(int** zbuffer);
