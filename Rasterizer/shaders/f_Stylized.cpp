#include "../core/include/gl_soft.h"
#include "../core/include/model.h"

class f_Stylized :public f_Shader {
public:
	f_Stylized() {}

	Vec3f lightDir;
	Model* model;

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
		TGAColor color = TGAColor(255, 155, 0);

		if (intensity == 0) {
			color = color * intensity;
			std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color[0]) ,float(color[1]) ,float(color[2]) ,float(color[3]) };
			return out;
		}

		if (intensity > .85) intensity = 1;
		else if (intensity > .60) intensity = .80;
		else if (intensity > .45) intensity = .60;
		else if (intensity > .30) intensity = .45;
		else if (intensity > .15) intensity = .30;
		else intensity = 0;

		color = color * intensity;

		std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color[0]) ,float(color[1]) ,float(color[2]) ,float(color[3]) };
		return out;
	}
};
