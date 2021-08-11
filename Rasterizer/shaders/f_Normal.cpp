#include "../core/include/gl_soft.h"
#include "../core/include/model.h"

class f_Normal :public f_Shader {
public:
	f_Normal() {}
	Matrix* mat_invert_transpose;
	Model* model;
	TGAImage* normalMap = nullptr;

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
			mat<3, 3, float> A;
			A[0] = model->vert(model->face(int(f_in[0]))[1 * 2]) - model->vert(model->face(int(f_in[0]))[0 * 2]);
			A[1] = model->vert(model->face(int(f_in[0]))[2 * 2]) - model->vert(model->face(int(f_in[0]))[0 * 2]);
			A[2] = normal;

			mat<3, 3, float> AI = A.invert();

			Vec3f i = AI * Vec3f(model->tex_[model->face(int(f_in[0]))[1 * 2 + 1]].x - model->tex_[model->face(int(f_in[0]))[0 * 2 + 1]].x, model->tex_[model->face(int(f_in[0]))[2 * 2 + 1]].x - model->tex_[model->face(int(f_in[0]))[0 * 2 + 1]].x, 0);
			Vec3f j = AI * Vec3f(model->tex_[model->face(int(f_in[0]))[1 * 2 + 1]].y - model->tex_[model->face(int(f_in[0]))[0 * 2 + 1]].y, model->tex_[model->face(int(f_in[0]))[2 * 2 + 1]].y - model->tex_[model->face(int(f_in[0]))[0 * 2 + 1]].y, 0);

			mat<3, 3, float> B;
			B.set_col(0, i.normalize());
			B.set_col(1, j.normalize());
			B.set_col(2, normal);

			// BGRA
			Vec3f normal_load = Vec3f(normalMap->get(int(T.x), int(T.y))[2] / 255.f * 2.0 - 1.0, normalMap->get(int(T.x), int(T.y))[1] / 255.f * 2.0 - 1.0, normalMap->get(int(T.x), int(T.y))[0] / 255.f * 2.0 - 1.0).normalize();
			normal = B * Vec3f(normal_load.x, normal_load.y, normal_load.z);
		}

		Vec4f normal_4 = *mat_invert_transpose * Vec4f(normal.x, normal.y, normal.z, 0.0f);
		normal = Vec3f(normal_4.x, normal_4.y, normal_4.z);
		normal.normalize();

		std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float((normal.x + 1.0) / 2.0 * 255.0) ,float((normal.y + 1.0) / 2.0 * 255.0) ,float((normal.z + 1.0) / 2.0 * 255.0) , 1 };
		return out;
	}
};
