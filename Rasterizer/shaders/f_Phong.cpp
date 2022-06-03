#include "../core/gl_soft.h"
#include "../core/model.h"

class f_Phong :public f_Shader {
public:
	f_Phong() {}

	Matrix* mat_invert_transpose;
	Matrix* shadowMat;
	Vec3f* lightDir;
	Vec3f* viewDir;
	Model* model;
	TGAImage* diffuse = nullptr;
	TGAImage* specular = nullptr;
	TGAImage* normalMap = nullptr;
	TGAImage* shadowMap = nullptr;
	TGAImage* ambientMap = nullptr;

	virtual std::vector<float> fragment(std::vector<float> f_in) override {
		Vec2f T(0, 0);
		for (int i = 0; i < 3; i++) {
			T.x += model->tex_[model->face(int(f_in[0]))[i * 2 + 1]].x * f_in[i + 4];
			T.y += model->tex_[model->face(int(f_in[0]))[i * 2 + 1]].y * f_in[i + 4];
		}

		Vec3f normal(0, 0, 0);

		for (int i = 0; i < 3; i++) {
			normal.x += model->norm(model->face(int(f_in[0]))[i * 2]).x * f_in[i + 4];
			normal.y += model->norm(model->face(int(f_in[0]))[i * 2]).y * f_in[i + 4];
			normal.z += model->norm(model->face(int(f_in[0]))[i * 2]).z * f_in[i + 4];
		}
		normal.normalize();
		if (normalMap != nullptr) {
			// 先构造原始的三角面坐标系矩阵, 这个矩阵可以将向量转为UV
			mat<3, 3, float> A;
			A[0] = model->vert(model->face(int(f_in[0]))[1 * 2]) - model->vert(model->face(int(f_in[0]))[0 * 2]);
			A[1] = model->vert(model->face(int(f_in[0]))[2 * 2]) - model->vert(model->face(int(f_in[0]))[0 * 2]);
			A[2] = normal;

			// 求逆使其可以用来将UV转为向量
			mat<3, 3, float> AI = A.invert();
			// 从而将对应面片的UV转为了ij基向量
			Vec3f i = AI * Vec3f(model->tex_[model->face(int(f_in[0]))[1 * 2 + 1]].x - model->tex_[model->face(int(f_in[0]))[0 * 2 + 1]].x, model->tex_[model->face(int(f_in[0]))[2 * 2 + 1]].x - model->tex_[model->face(int(f_in[0]))[0 * 2 + 1]].x, 0);
			Vec3f j = AI * Vec3f(model->tex_[model->face(int(f_in[0]))[1 * 2 + 1]].y - model->tex_[model->face(int(f_in[0]))[0 * 2 + 1]].y, model->tex_[model->face(int(f_in[0]))[2 * 2 + 1]].y - model->tex_[model->face(int(f_in[0]))[0 * 2 + 1]].y, 0);

			// 配合当前的面法线, 组成Darboux基矩阵(但是这个基是不是并不正交?
			mat<3, 3, float> B;
			B.set_col(0, i.normalize());
			B.set_col(1, j.normalize());
			B.set_col(2, normal);

			// 读取法线贴图中保存的法线, 相乘就实现了基变换, 也就是将本来定义在UVZ中的法线转到了TBN坐标系中
			Vec3f normal_load = Vec3f(normalMap->get(int(T.x), int(T.y))[2] / 255.f * 2.0 - 1.0, normalMap->get(int(T.x), int(T.y))[1] / 255.f * 2.0 - 1.0, normalMap->get(int(T.x), int(T.y))[0] / 255.f * 2.0 - 1.0).normalize();
			// 这一句就是坐标系变换
			normal = B * Vec3f(normal_load.x, normal_load.y, normal_load.z);
		}

		Vec4f normal_4 = (*mat_invert_transpose) * Vec4f(normal.x, normal.y, normal.z, 0.0f);
		normal = Vec3f(normal_4.x, normal_4.y, normal_4.z);
		normal.normalize();

		float diff = std::max(0.f, normal * (*lightDir));
		TGAColor color = TGAColor(255, 255, 255);

		float spec = 0;
		Vec3f reflect = (normal * (normal * (*lightDir) * 2.f) - (*lightDir)).normalize();

		float shadow = 1;

		float ambient = 5;

		if (diffuse != nullptr) {
			color = diffuse->get(int(T.x), int(T.y));
		}

		if (specular != nullptr) {
			spec = pow(std::max((*viewDir) * reflect, 0.0f), specular->get(int(T.x), int(T.y))[0]);
		}

		if (shadowMap != nullptr) {
			Vec4f ori = Vec4f(f_in[7], f_in[8], f_in[9], 1.0f);
			Vec4f p_in_shadow = (*shadowMat) * ori;
			p_in_shadow = p_in_shadow / p_in_shadow[3];
			shadow = .3 + .7 * (float(shadowMap->get(int(p_in_shadow[0]), int(p_in_shadow[1]))[0]) < p_in_shadow[2] + 43.34);
		}

		if (ambientMap != nullptr) {
			ambient = ambient * 2 * ambientMap->get(int(T.x), int(T.y))[0] / 255;
		}

		for (int i = 0; i < 3; i++) {
			// ambient + diffuse + sepcular
			color[i] = std::min<float>(ambient + color[i] * shadow * (1.2 * diff + .6 * spec), 255);
		}

		std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color[0]) ,float(color[1]) ,float(color[2]) ,float(color[3]) };
		return out;
	}
};
