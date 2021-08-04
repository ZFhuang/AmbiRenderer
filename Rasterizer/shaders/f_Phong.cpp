#include "../core/include/gl_soft.h"
#include "../core/include/model.h"

class f_Phong :public f_Shader {
public:
	f_Phong() {}

	Matrix* mat_invert_transpose;
	Vec3f lightDir;
	Vec3f viewDir;
	Model* model;
	TGAImage* diffuse = nullptr;
	TGAImage* specular = nullptr;
	TGAImage* normalMap = nullptr;

	virtual std::vector<float> fragment(std::vector<float> f_in) override {
		Vec2f T(0, 0);
		for (int i = 0; i < 3; i++) {
			T.x += model->tex_[model->face(int(f_in[0]))[i * 2 + 1]].x * f_in[i + 4];
			T.y += model->tex_[model->face(int(f_in[0]))[i * 2 + 1]].y * f_in[i + 4];
		}

		Vec3f normal(0, 0, 0);

		if (normalMap == nullptr) {
			for (int i = 0; i < 3; i++) {
				normal.x += model->norm(model->face(int(f_in[0]))[i * 2]).x * f_in[i + 4];
				normal.y += model->norm(model->face(int(f_in[0]))[i * 2]).y * f_in[i + 4];
				normal.z += model->norm(model->face(int(f_in[0]))[i * 2]).z * f_in[i + 4];
			}
		}
		else {
			// BGRA
			Vec3f normal_load = Vec3f(normalMap->get(int(T.x), int(T.y))[2] / 255.f * 2.0 - 1.0, normalMap->get(int(T.x), int(T.y))[1] / 255.f * 2.0 - 1.0, normalMap->get(int(T.x), int(T.y))[0] / 255.f * 2.0 - 1.0);
			Vec4f normal_4 = *mat_invert_transpose * Vec4f(normal_load.x, normal_load.y, normal_load.z, 0.0f);
			normal = Vec3f(normal_4.x, normal_4.y, normal_4.z);
		}
		normal.normalize();

		float diff = std::max(0.f, normal * lightDir);
		TGAColor color = TGAColor(255, 255, 255);

		float spec = 0;
		Vec3f reflect = (normal * (normal * lightDir * 2.f) - lightDir).normalize();

		if (diffuse != nullptr) {
			color = diffuse->get(int(T.x), int(T.y));
		}

		if (specular != nullptr) {
			spec = pow(std::max(viewDir * reflect, 0.0f), specular->get(int(T.x), int(T.y))[0]);
		}

		for (int i = 0; i < 3; i++) {
			// ambient + diffuse + sepcular
			color[i] = std::min<float>(5 + color[i] * (diff + 0.6 * spec), 255);
		}

		std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color[0]) ,float(color[1]) ,float(color[2]) ,float(color[3]) };
		return out;
	}
};
