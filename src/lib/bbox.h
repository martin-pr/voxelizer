#pragma once

#include <array>
#include <iostream>

#include <boost/serialization/access.hpp>

struct bbox {
	bbox(const std::array<float, 3>& _min, const std::array<float, 3>& _max) : min(_min), max(_max) {
	}

	std::array<float, 3> min, max;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & min;
		ar & max;
	}
};

inline std::ostream& operator << (std::ostream& out, const bbox& b) {
	for(unsigned char a=0;a<3;++a)
		out << b.min[a] << " - " << b.max[a] << "  ";

	return out;
}

