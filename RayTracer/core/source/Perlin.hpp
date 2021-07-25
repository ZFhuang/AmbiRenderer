#pragma once

#include"RTweekend.hpp"
#include"Vec3.hpp"

// ��������Perlin��������, Perlin��������ɫ���ظ���, 
// Ҳ���ǽ���α����������ڲ��������Ĳ�����ͬʱ, �õ���ͬ������
// ��ʹ�������������������Ե�ͬʱ�ֲ��ϸ���ƽ��

class Perlin {
public:
	Perlin() {
		// ���ɴ���ͨ�±굽(0-1)�������, ������ΪĿ��Perlin������α���ӳ��
		// ����ʹ��ԭʼ�汾�򵥵����������, ��λ������˵õ��������ֵ���ȶ��Ҳ�ֵ��Ҳ��ƽ��
		randVec = new Vec3[pointCount];
		for (int i = 0; i < pointCount; ++i) {
			randVec[i] = random_in_unit_sphere();
		}
		// ��������ȡ�±��������������, ���Һ�̶�ס�Ӷ��õ�α�����, ��Ϊhash����
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

	// ����Perlin����
	double noise(const Point3& p)const {
		// �ȼ������ǰ����ӳ�䵽Perlin�����ϵ�����, ijk��Ŀ�����ڵľ�������, uvw��������
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());
		auto i = static_cast<int>(floor(p.x()));
		auto j = static_cast<int>(floor(p.y()));
		auto k = static_cast<int>(floor(p.z()));
		Vec3 c[2][2][2] = {};
		// �õ���Χxyz8�����������ӳ��
		for (int di = 0; di < 2; ++di) {
			for (int dj = 0; dj < 2; ++dj) {
				for (int dk = 0; dk < 2; ++dk) {
					// ��ֵӳ�䵽255�в��ϲ�, ���ö�Ӧ��α����±�������������в���
					c[di][dj][dk] = randVec[
						permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]
					];
				}
			}
		}
		// �������Բ�ֵ��������ƽ��һЩ, ����ķ���ֵ�и�ֵ��Ҫ����
		return perlin_interp(c, u, v, w);
	}

	// ����, ����һ���ĵ���������Ȩ�õ���Perlin����, ʹ�������߽��������, ��������
	double turbulence(const Point3& p, int depth = 7) const {
		auto accum = 0.0;
		auto temp_p = p;
		auto weight = 1.0;
		for (int i = 0; i < depth; ++i) {
			accum += weight * noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}
		// ֱ�ӷ��ؾ���ֵ�������ⲿ�Ĺ�һ������
		return fabs(accum);
	}

private:
	// ��������Ϊ256��
	static const int pointCount = 256;
	// �������ӳ������
	Vec3* randVec;
	// hash�±�����
	int* permX;
	int* permY;
	int* permZ;

	// ����������������, ������Ϊһ��hash����
	static int* perlin_generate_perm() {
		// ��������ͨ��˳������
		auto p = new int[pointCount];
		for (int i = 0; i < Perlin::pointCount; ++i) {
			p[i] = i;
		}
		// �ٽ����������
		permute(p, pointCount);
		return p;
	}

	// ����������������е�ֵ��������
	static void permute(int* p, int n) {
		for (int i = n - 1; i > 0; --i) {
			// �����������������Ԫ��
			int target = random_int(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	// Perlin��ֵ, ������������ڽ�����λ�������в�ֵ
	static double perlin_interp(Vec3 c[2][2][2], double u, double v, double w) {
		// ����Hermitian����ƽ���ò�ֵ�õ������겻Ҫ����ôǿ�������
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);
		auto accum = 0.0;
		// ��������ά8��������
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					// ���㵱ǰ��ƫ�뾧��ǵ�������ΪȨ������
					Vec3 weight_v(u - i, v - j, w - k);
					// ��������������Բ�ֵ(��ǰ�����һ��)��Ϊ��һ��Ȩ��
					// Ȼ����α��������ĵ�λ��������е��
					accum += (i * uu + (1 - i) * (1 - uu)) *
						(j * vv + (1 - j) * (1 - vv)) *
						(k * ww + (1 - k) * (1 - ww)) *
						dot(c[i][j][k], weight_v);
				}
			}
		}
		return accum;
	}

	// ������ƽ����ֵ
	static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
		auto accum = 0.0;
		// ��������ά8��������
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					// ʹ�������������uvw�����������Բ�ֵ(��ǰ�����һ��)
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