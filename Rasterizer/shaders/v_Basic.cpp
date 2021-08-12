#include "../core/include/gl_soft.h"
#include "../core/include/model.h"

class v_Basic :public v_Shader {
public:
	v_Basic() {}

	Matrix* vpm_mat;
	Model* model;

	virtual std::vector<float> vertex(int f_idx, int v_idx) override {
		std::vector<float> out;
		Vec3f p_in = model->vert(v_idx);
		Vec4f point = (*vpm_mat) * Vec4f(p_in.x, p_in.y, p_in.z, 1.0f);
		out.push_back(point.x / point.w);
		out.push_back(point.y / point.w);
		out.push_back(point.z / point.w);
		out.push_back(p_in.x);
		out.push_back(p_in.y);
		out.push_back(p_in.z);
		return out;
	}
};
