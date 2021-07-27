#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) : verts_(), faces_() {
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
			for (int i = 0; i < 3; i++) iss >> v.raw[i];
			verts_.push_back(v);
		}
		// 当以[f空格]开头, 代表是面, 压入面数组
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> f;
			int itrash, idx;
			iss >> trash;
			// 利用连续读入流来读取, 好写法
			while (iss >> idx >> trash >> itrash >> trash >> itrash) {
				idx--; // in wavefront obj all indices start at 1, not zero
				// 压入面的顶点序号
				f.push_back(idx);
			}
			// 压入拥有顶点序号的面
			faces_.push_back(f);
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

std::vector<int> Model::face(int idx) {
	return faces_[idx];
}

Vec3f Model::vert(int i) {
	return verts_[i];
}
