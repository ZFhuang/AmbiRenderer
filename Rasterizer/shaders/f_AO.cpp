#include "../core/include/gl_soft.h"
#include "../core/include/model.h"

class f_AO :public f_Shader {
public:
	f_AO() {}

	Model* model;
	TGAImage* shadowMap = nullptr;
	TGAImage* AOMap = nullptr;

	virtual std::vector<float> fragment(std::vector<float> f_in) override {
		Vec2f T(0, 0);
		for (int i = 0; i < 3; i++) {
			T.x += model->tex_[model->face(int(f_in[0]))[i * 2 + 1]].x * f_in[i + 4];
			T.y += model->tex_[model->face(int(f_in[0]))[i * 2 + 1]].y * f_in[i + 4];
		}

		if (float(shadowMap->get(int(f_in[1]), int(f_in[2]))[0]) > f_in[3]) {
			AOMap->set(int(T.x), int(T.y), TGAColor(255));
		}

		TGAColor color = TGAColor(f_in[3], f_in[3], f_in[3], 255);
		std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color[0]) ,float(color[1]) ,float(color[2]) ,float(color[3]) };
		return out;
	}
};
