#include "../core/include/gl_soft.h"

class f_SSAO :public f_Shader {
public:
	f_SSAO() {}

	virtual std::vector<float> fragment(std::vector<float> f_in) override {
		TGAColor color = TGAColor(255, 255, 255, 255);
		std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color[0]) ,float(color[1]) ,float(color[2]) ,float(color[3]) };
		return out;
	}
};
