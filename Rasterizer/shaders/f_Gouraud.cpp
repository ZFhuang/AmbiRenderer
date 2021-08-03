#include "../core/include/gl_soft.h"

class f_Gouraud :public f_Shader {
public:
	f_Gouraud() {}

	virtual std::vector<float> fragment(std::vector<float> f_in) override {
		TGAColor color = TGAColor(f_in[3], f_in[3], f_in[3], 255);
		std::vector<float> out = { f_in[1] ,f_in[2] ,f_in[3], float(color.r) ,float(color.g) ,float(color.b) ,float(color.a) };
		return out;
	}
};
