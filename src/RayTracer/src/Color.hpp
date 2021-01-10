#pragma once

#include"Vec3.hpp"
#include<iostream>

// ר�ŵ�color�ļ�����������Щ����ɫ�йصĺ���

// ��װ��pixel�е���ɫ����д���Ӧ����out��, ͨ����out�ض����ļ�
void write_color(std::ostream& out, Color pixel) {
	out << static_cast<int>(255.999 * pixel.x()) << ' '
		<< static_cast<int>(255.999 * pixel.y()) << ' '
		<< static_cast<int>(255.999 * pixel.z()) << '\n';
}