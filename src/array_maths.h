#pragma once

#include <array>

std::array<float, 3> operator + (const std::array<float, 3>& o1, const std::array<float, 3>& o2);
std::array<float, 3> operator - (const std::array<float, 3>& o1, const std::array<float, 3>& o2);
std::array<float, 3> operator / (const std::array<float, 3>& o1, const float& o2);
std::array<float, 3> operator - (const std::array<float, 3>& o);
