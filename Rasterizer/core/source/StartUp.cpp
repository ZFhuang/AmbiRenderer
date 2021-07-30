#include"../include/tgaimage.h"
#include"../include/model.h"
#include<utility>
#include<cstdlib>
#include<string>
#include <limits>
#include <vector>
#include <cmath>

namespace PATH
{
	const std::string RESOURCES = "C:/Work/AmbiRenderer/Resources/";
}

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const int width = 800;
const int height = 800;

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
	// 计算三角形ABC中点P对应的重心坐标
	// 任意两边的向量
	Vec3f s[2];
	for (int i = 2; i--; ) {
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	// 叉乘得到的第三轴调整后就是重心坐标
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void initTextureCoord(Model* model, TGAImage& texture) {
	int tex_width = texture.get_width();
	int tex_height = texture.get_height();
	for (int i = 0; i < model->ntex(); i++) {
		model->tex_[i].x *= tex_width;
		model->tex_[i].y *= tex_height;
	}
}

void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color) {
	int x0 = t0.x;
	int y0 = t0.y;
	int x1 = t1.x;
	int y1 = t1.y;
	// 按照长边遍历
	bool isSteep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		isSteep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	// 从小值遍历到大值
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	// 用长短边的累计误差来替代0.5的浮点检测, 乘2使得分界点变为1
	int yerror = std::abs(y1 - y0) * 2;
	int xerror = x1 - x0;
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		// 由于长边交换, 因此绘制顺序需要换回来
		if (isSteep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error += yerror;
		if (error > xerror) {
			// 两种方向的步进
			y += y0 > y1 ? -1 : 1;
			error -= xerror * 2;
		}
	}
}

void triangle(Vec3f* pts, float** zbuffer, TGAImage& image, float intensity, TGAImage& texture, Vec2f* texPts) {
	// 绘制三角形
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
	// 获取三角形的二维包围盒
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}
	Vec3f P;
	Vec2f T;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			// 通过重心坐标判断当前像素在不在三角形内
			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			// 利用重心坐标插值三个顶点的z值得到目标像素的z值
			for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];

			if (zbuffer[int(P.x)][int(P.y)] < P.z) {
				// 在当前的Z缓冲前面时就要进行绘制
				zbuffer[int(P.x)][int(P.y)] = P.z;
				// 如果要渲染Z缓冲就使用下面的颜色
				TGAColor z_color(int((P.z + 1) / 2 * 255), int((P.z + 1) / 2 * 255), int((P.z + 1) / 2 * 255), 255);
				// 从材质取值
				// 利用重心坐标插值三个材质坐标得到目标像素的真实材质坐标
				T.x = 0;
				T.y = 0;
				for (int i = 0; i < 3; i++) {
					T.x += texPts[i][0] * bc_screen[i];
					T.y += texPts[i][1] * bc_screen[i];
				}
				// 读取对应的材质颜色并与光照加权
				TGAColor color = texture.get(int(T.x), int(T.y));
				color.r *= intensity;
				color.g *= intensity;
				color.b *= intensity;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void rasterize(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color, int ybuffer[]) {
	// 思路和光栅化差不多
	if (p0.x > p1.x) {
		std::swap(p0, p1);
	}
	for (int x = p0.x; x <= p1.x; x++) {
		// 线性插值得到y值
		float t = (x - p0.x) / (float)(p1.x - p0.x);
		int y = p0.y * (1. - t) + p1.y * t;
		// 只保留最大的y(离相机最近)
		if (ybuffer[x] < y) {
			ybuffer[x] = y;
			image.set(x, 0, color);
		}
	}
}

Vec3f world2screen(Vec3f v) {
	// 正交投影的转换方法, 此时的裁剪体是[-1, 1], 改变xy但是不改变z, 反之z只是一个相对值
	return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

float** initZBuffer() {
	// 初始化Z缓冲为最大值
	float** zbuffer = new float* [height];
	for (int line = 0; line < height; line++) {
		zbuffer[line] = new float[width];
	}
	for (int line = 0; line < height; line++) {
		for (int item = 0; item < width; item++) {
			zbuffer[line][item] = -std::numeric_limits<float>::max();
		}
	}
	return zbuffer;
}

void deleteZBuffer(float** zbuffer) {
	for (int line = 0; line < height; line++) {
		delete zbuffer[line];
	}
	delete zbuffer;
}

void render(Model* model, TGAImage& image, Vec3f light_dir, float** zbuffer, TGAImage& texture, Matrix viewProjMat) {
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		// 计算光照因子
		Vec3f n = cross(model->vert(face[1 * 2]) - model->vert(face[0 * 2]), model->vert(face[2 * 2]) - model->vert(face[0 * 2]));
		n.normalize();
		float intensity = -(n * light_dir);
		Vec3f pts[3];
		// 三角形的每个面都需要转到屏幕空间中才能进行Z缓冲渲染
		for (int i = 0; i < 3; i++) {
			Vec4f v = viewProjMat * Vec4f(model->vert(face[i * 2]).x, model->vert(face[i * 2]).y, model->vert(face[i * 2]).z, 1.0f);
			pts[i] = world2screen(Vec3f(v.x / v.w, v.y / v.w, v.z / v.w));
		}

		// 获得顶点材质坐标
		Vec2f texPts[3];
		for (int i = 0; i < 3; i++) texPts[i] = model->tex_[face[i * 2 + 1]];
		if (intensity > 0) {
			triangle(pts, zbuffer, image, intensity, texture, texPts);
		}
	}
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

Matrix makeViewProjMat(Vec3f xyz) {
	Matrix mat = Matrix::identity();
	if (xyz.x != 0)
		mat[3][0] = -1 / xyz.x;
	if (xyz.y != 0)
		mat[3][1] = -1 / xyz.y;
	if (xyz.z != 0)
		mat[3][2] = -1 / xyz.z;
	return mat;
}

Vec3f calModelCenter(Model* model) {
	Vec3f c = Vec3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < model->nverts(); i++) {
		c = c + model->vert(i);
	}
	c = c / model->nverts();
	return c;
}

void applyModelMats(Matrix& mat, Model* model) {
	for (int i = 0; i < model->nverts(); i++) {
		Vec4f v = mat * Vec4f(model->vert(i).x, model->vert(i).y, model->vert(i).z, 1.0f);
		model->verts_[i] = Vec3f(v.x, v.y, v.z);
	}
}

void applyViewMats(Matrix& mat, Model* model) {
	for (int i = 0; i < model->nverts(); i++) {
		Vec4f v = mat * Vec4f(model->vert(i).x, model->vert(i).y, model->vert(i).z, 1.0f);
		model->verts_[i] = Vec3f(v.x / v.w, v.y / v.w, v.z / v.w);
	}
}

int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);
	TGAImage texture;
	bool tex_success = texture.read_tga_file((PATH::RESOURCES + "african_head_diffuse.tga").c_str());
	assert(tex_success);
	texture.flip_vertically();
	Model* model = new Model((PATH::RESOURCES + "african_head.obj").c_str());
	initTextureCoord(model, texture);
	Vec3f light_dir(0, 0, -1);

	// 模型变换矩阵
	//Matrix transMat = makeModelTranslationMat(calModelCenter(model));
	//Matrix scaleMat = makeModelScalingMat(Vec3f(2, 1, 1));
	//Matrix rotateMat = makeModelRotationMat(Vec3f(1, 1, 1));
	//Matrix neg_transMat = transMat.invert();
	//Matrix transformMat = transMat * rotateMat * scaleMat * neg_transMat;
	Matrix transMat = makeModelTranslationMat(Vec3f(0, 0, -0.5));
	Matrix transformMat = transMat;
	applyModelMats(transformMat, model);

	// 视图变换矩阵
	Matrix projMat = makeViewProjMat(Vec3f(0, 0, 2));

	float** zbuffer = initZBuffer();
	render(model, image, light_dir, zbuffer, texture, projMat);

	image.flip_vertically();
	image.write_tga_file("output.tga");

	delete model;
	deleteZBuffer(zbuffer);
	return 0;
}
