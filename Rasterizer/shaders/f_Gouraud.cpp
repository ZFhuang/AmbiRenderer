#include "../core/include/gl_soft.h"
#include "../core/include/model.h"

class f_Gouraud :public f_Shader {
public:
	f_Gouraud() {}

	Vec3f lightDir;
	Model* model;
	TGAImage* texture;

	virtual std::vector<float> fragment(std::vector<float> f_in) override {
		Vec3f N(0, 0, 0);
		for (int i = 0; i < 3; i++) {
			N.x += model->norm(model->face(int(f_in[0]))[i * 2]).x * f_in[i + 4];
			N.y += model->norm(model->face(int(f_in[0]))[i * 2]).y * f_in[i + 4];
			N.z += model->norm(model->face(int(f_in[0]))[i * 2]).z * f_in[i + 4];
		}
		N.normalize();

		float intensity = N * lightDir.normalize();
		intensity = intensity > 0 ? intensity : 0;
		TGAColor color = TGAColor(255, 255, 255);

		if (intensity == 0) {
			color = color * intensity;
			std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color[0]) ,float(color[1]) ,float(color[2]) ,float(color[3]) };
			return out;
		}

		if (texture != nullptr) {
			Vec2f T(0, 0);
			for (int i = 0; i < 3; i++) {
				T.x += model->tex_[model->face(int(f_in[0]))[i * 2 + 1]].x * f_in[i + 4];
				T.y += model->tex_[model->face(int(f_in[0]))[i * 2 + 1]].y * f_in[i + 4];
			}
			color = texture->get(int(T.x), int(T.y));
		}

		color = color * intensity;

		std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color[0]) ,float(color[1]) ,float(color[2]) ,float(color[3]) };
		return out;
	}
};
