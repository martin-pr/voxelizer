#include "array_maths.h"

std::array<float, 3> operator + (const std::array<float, 3>& o1, const std::array<float, 3>& o2) {
	return std::array<float, 3>{{o1[0]+o2[0], o1[1]+o2[1], o1[2]+o2[2]}};
}

std::array<float, 3> operator - (const std::array<float, 3>& o1, const std::array<float, 3>& o2) {
	return std::array<float, 3>{{o1[0]-o2[0], o1[1]-o2[1], o1[2]-o2[2]}};
}

std::array<float, 3> operator / (const std::array<float, 3>& o1, const float& o2) {
	return std::array<float, 3>{{o1[0]/o2, o1[1]/o2, o1[2]/o2}};
}

std::array<float, 3> operator - (const std::array<float, 3>& o) {
	return std::array<float, 3>{{-o[0], -o[1], -o[2]}};
}
