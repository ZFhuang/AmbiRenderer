#include "gl_soft.h"

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
	// 计算三角形ABC中点P对应的重心坐标
	// 任意两边的向量
	Vec3f s[2];
	for (int i = 2; i--; ) {
		s[i][0] = int(C[i]) - int(A[i]);
		s[i][1] = int(B[i]) - int(A[i]);
		s[i][2] = int(A[i]) - int(P[i]);
	}
	// 叉乘得到的第三轴调整后就是重心坐标
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

Matrix makeModelRotationMat(Vec3f xyz) {
	Matrix matX = Matrix::identity();
	matX.set_col(1, Vec4f(0.0f, cos(xyz.x), sin(xyz.x), 0.0f));
	matX.set_col(2, Vec4f(0.0f, -sin(xyz.x), cos(xyz.x), 0.0f));
	Matrix matY = Matrix::identity();
	matY.set_col(0, Vec4f(cos(xyz.y), 0.0f, -sin(xyz.y), 0.0f));
	matY.set_col(2, Vec4f(sin(xyz.y), 0.0f, cos(xyz.y), 0.0f));
	Matrix matZ = Matrix::identity();
	matZ.set_col(0, Vec4f(cos(xyz.z), sin(xyz.z), 0.0f, 0.0f));
	matZ.set_col(1, Vec4f(-sin(xyz.z), cos(xyz.z), 0.0f, 0.0f));
	return matZ * matY * matX;
}

Matrix makeModelTranslationMat(Vec3f xyz) {
	Matrix mat = Matrix::identity();
	mat.set_col(3, Vec4f(xyz.x, xyz.y, xyz.z, 1.0f));
	return mat;
}

Matrix makeModelScalingMat(Vec3f xyz) {
	Matrix mat = Matrix::identity();
	mat[0][0] = xyz.x;
	mat[1][1] = xyz.y;
	mat[2][2] = xyz.z;
	return mat;
}

Matrix makeProjMat(float coeff = 0) {
	Matrix mat = Matrix::identity();
	if (coeff != 0)
		mat[3][2] = -1 / coeff;
	return mat;
}

Matrix makeViewMat(Vec3f eye, Vec3f center, Vec3f up) {
	// 先构建目标坐标系的三个轴
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();
	// 构造方向矩阵和移动矩阵 
	Matrix Minv = Matrix::identity();
	Matrix Tr = Matrix::identity();
	for (int i = 0; i < 3; i++) {
		// 方向矩阵部分
		Minv[0][i] = x[i];
		Minv[1][i] = y[i];
		Minv[2][i] = z[i];
		// 视点移动矩阵部分
		Tr[i][3] = -center[i];
	}
	// 相乘得到相机变换矩阵
	return Minv * Tr;
}

Matrix makeViewportMat(int x, int y, int w, int h) {
	Matrix mat = Matrix::identity();
	mat[0][3] = x + w / 2.f;
	mat[1][3] = y + h / 2.f;
	mat[2][3] = depth / 2.f;

	mat[0][0] = w / 2.f;
	mat[1][1] = h / 2.f;
	mat[2][2] = depth / 2.f;
	return mat;
}

float** makeZBuffer(int height, int width) {
	// 初始化Z缓冲为最大值
	float** zbuffer = new float* [height];
	for (int line = 0; line < height; line++) {
		zbuffer[line] = new float[width];
	}
	for (int line = 0; line < height; line++) {
		for (int item = 0; item < width; item++) {
			zbuffer[line][item] = std::numeric_limits<unsigned int>::min();
		}
	}
	return zbuffer;
}

void deleteZBuffer(float** zbuffer, int height) {
	for (int line = 0; line < height; line++) {
		delete zbuffer[line];
	}
	delete zbuffer;
	zbuffer = nullptr;
}

Vec3f rand_point_on_unit_sphere() {
	float u = (float)rand() / (float)RAND_MAX;
	float v = (float)rand() / (float)RAND_MAX;
	float theta = 2.f * M_PI * u;
	float phi = acos(2.f * v - 1.f);
	return Vec3f(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
}

float max_elevation_angle(float** zbuffer, Vec2f p, Vec2f dir) {
	float maxangle = 0;
	// 射线检测, 在dir上步进1000次, 注意dir是二维的, 是在深度图上进行的步进
	// 因此理解为判断能被原处射到的某点的最大角度
	for (float t = 0.; t < 100; t += 1.) {
		Vec2f cur = p + dir * t;
		if (cur.x >= width || cur.y >= height || cur.x < 0 || cur.y < 0) return maxangle;

		float distance = (p - cur).norm();
		if (distance < 1.f) continue;
		// 每次遇到更大的z值时, 利用距离和高度差更新角度
		float elevation = (zbuffer[int(cur.y)][int(cur.x)] - zbuffer[int(p.y)][int(p.x)]);
		maxangle = std::max(maxangle, atanf(elevation / distance));
	}
	return maxangle;
}
