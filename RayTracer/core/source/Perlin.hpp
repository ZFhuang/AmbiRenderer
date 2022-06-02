#pragma once

#include"RTweekend.hpp"
#include"Vec3.hpp"

// 用于生成Perlin噪声的类, Perlin噪声的特色是重复性, 
// 也就是借助伪随机数让用于产生噪声的参数相同时, 得到相同的噪声
// 这使得噪声在整体存在随机性的同时局部上更加平滑

class Perlin {
public:
	Perlin() {
		// 生成从普通下标到(0-1)随机数组, 用来作为目标Perlin噪声的伪随机映射
		// 不是使用原始版本简单的随机浮点数, 单位向量点乘得到的随机量值更稳定且插值后也更平滑
		randVec = new Vec3[pointCount];
		for (int i = 0; i < pointCount; ++i) {
			randVec[i] = random_in_unit_sphere();
		}
		// 生成用于取下标的三个乱序数组, 打乱后固定住从而得到伪随机性, 作为hash数组
		permX = perlin_generate_perm();
		permY = perlin_generate_perm();
		permZ = perlin_generate_perm();
	}

	~Perlin() {
		delete[] randVec;
		delete[] permX;
		delete[] permY;
		delete[] permZ;
	}

	// 返回Perlin噪声
	double noise(const Point3& p)const {
		// 先计算出当前坐标映射到Perlin材质上的坐标, ijk是目标所在的晶格坐标, uvw是子坐标
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());
		auto i = static_cast<int>(floor(p.x()));
		auto j = static_cast<int>(floor(p.y()));
		auto k = static_cast<int>(floor(p.z()));
		Vec3 c[2][2][2] = {};
		// 得到周围xyz8个晶格的坐标映射
		for (int di = 0; di < 2; ++di) {
			for (int dj = 0; dj < 2; ++dj) {
				for (int dk = 0; dk < 2; ++dk) {
					// 将值映射到255中并合并, 调用对应的伪随机下标来从随机噪声中采样
					c[di][dj][dk] = randVec[
						permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]
					];
				}
			}
		}
		// 用三线性插值来让噪声平滑一些, 这里的返回值有负值需要处理
		return perlin_interp(c, u, v, w);
	}

	// 湍流, 按照一定的迭代次数加权得到的Perlin噪声, 使得噪声边界更加明显, 形似湍流
	double turbulence(const Point3& p, int depth = 7) const {
		auto accum = 0.0;
		auto temp_p = p;
		auto weight = 1.0;
		for (int i = 0; i < depth; ++i) {
			accum += weight * noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}
		// 直接返回绝对值避免了外部的归一化操作
		return fabs(accum);
	}

private:
	// 令噪声分为256种
	static const int pointCount = 256;
	// 随机噪声映射数组
	Vec3* randVec;
	// hash下标数组
	int* permX;
	int* permY;
	int* permZ;

	// 打乱现有数组数组, 用来作为一种hash函数
	static int* perlin_generate_perm() {
		// 先生成普通的顺序数组
		auto p = new int[pointCount];
		for (int i = 0; i < Perlin::pointCount; ++i) {
			p[i] = i;
		}
		// 再将其打乱重排
		permute(p, pointCount);
		return p;
	}

	// 根据随机数对数组中的值重新排列
	static void permute(int* p, int n) {
		for (int i = n - 1; i > 0; --i) {
			// 随机交换数组中两个元素
			int target = random_int(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	// Perlin插值, 对随机出来的邻近晶格单位向量进行插值
	static double perlin_interp(Vec3 c[2][2][2], double u, double v, double w) {
		// 进行Hermitian立方平滑让插值得到的坐标不要有那么强的网格感
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);
		auto accum = 0.0;
		// 在相邻三维8个晶格中
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					// 计算当前点偏离晶格角的向量作为权重向量
					Vec3 weight_v(u - i, v - j, w - k);
					// 对坐标进行三线性插值(当前格和下一格)作为进一步权重
					// 然后与伪随机产生的单位向量组进行点乘
					accum += (i * uu + (1 - i) * (1 - uu)) *
						(j * vv + (1 - j) * (1 - vv)) *
						(k * ww + (1 - k) * (1 - ww)) *
						dot(c[i][j][k], weight_v);
				}
			}
		}
		return accum;
	}

	// 三线性平滑插值
	static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
		auto accum = 0.0;
		// 在相邻三维8个晶格中
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					// 使用输入的子坐标uvw来进行三线性插值(当前格和下一格)
					accum += (i * u + (1 - i) * (1 - u)) *
						(j * v + (1 - j) * (1 - v)) *
						(k * w + (1 - k) * (1 - w)) *
						c[i][j][k];
				}
			}
		}
		return accum;
	}
};