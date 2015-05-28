#pragma once

#include <array>

#include <boost/serialization/access.hpp>

std::array<float, 3> operator + (const std::array<float, 3>& o1, const std::array<float, 3>& o2);
std::array<float, 3> operator - (const std::array<float, 3>& o1, const std::array<float, 3>& o2);
std::array<float, 3> operator / (const std::array<float, 3>& o1, const float& o2);
std::array<float, 3> operator - (const std::array<float, 3>& o);
const float squaredLength(const std::array<float, 3>& p);


namespace std {

template<class Archive, typename T, size_t SIZE>
inline void serialize(Archive& ar, std::array<T, SIZE>& t, const unsigned int file_version) {
	for(size_t a=0; a<SIZE; ++a)
		ar & t[a];
}

}
