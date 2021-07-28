#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) : verts_(), faces_(), tex_() {
	// 分析Obj文件
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		// 逐行读取模型
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		// 当以[v空格]开头, 代表是顶点, 压入顶点数组
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts_.push_back(v);
		}
		// 当以[f空格]开头, 代表是面, 压入面数组
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> f;
			int t_idx, v_idx, itrash;
			iss >> trash;
			// 利用连续读入流来读取, 好写法
			while (iss >> v_idx >> trash >> t_idx >> trash >> itrash) {
				v_idx--; // in wavefront obj all indices start at 1, not zero
				t_idx--;
				// 压入面的顶点序号
				f.push_back(v_idx);
				f.push_back(t_idx);
			}
			// 压入拥有顶点序号的面
			faces_.push_back(f);
		}
		else if (!line.compare(0, 4, "vt  ")) {
			iss >> trash >> trash;
			Vec2f vt;
			for (int i = 0; i < 2; i++) iss >> vt[i];
			tex_.push_back(vt);
		}
	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
	return (int)verts_.size();
}

int Model::nfaces() {
	return (int)faces_.size();
}

int Model::ntex()
{
	return (int)tex_.size();
}

std::vector<int> Model::face(int idx) {
	return faces_[idx];
}

Vec3f Model::vert(int i) {
	return verts_[i];
}
