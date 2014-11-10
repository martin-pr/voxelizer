#include "grid.h"

#include <cmath>
#include <cassert>
#include <iostream>

#include "array_maths.h"

using std::cout;
using std::endl;

grid::grid(unsigned char depth, const ::bbox& box) : m_bbox(box), m_data(depth-1) {
	m_depthPow = std::pow(2.0f, depth);
	m_coordLimit = (unsigned)m_depthPow - 1;
}

const std::array<unsigned, 3> grid::coord(const std::array<float, 3>& pos) const {
	std::array<unsigned, 3> upos;

	for(unsigned a=0;a<3;++a)
		upos[a] = std::min(
			(unsigned)std::max(0.0f, floorf((pos[a] - m_bbox.min[a]) / (m_bbox.max[a] - m_bbox.min[a]) * m_depthPow)),
			m_coordLimit);

	assert((upos[0] <= m_coordLimit) && (upos[1] <= m_coordLimit) && (upos[2] <= m_coordLimit));

	return upos;
}

void grid::set(const std::array<float, 3>& pos) {
	set(coord(pos));
}

void grid::set(const std::array<unsigned, 3>& pos) {
	m_data.set(pos[0], pos[1], pos[2]);
}

const bool grid::get(const std::array<float, 3>& pos) const {
	return get(coord(pos));
}

const bool grid::get(const std::array<unsigned, 3>& pos) const {
	return m_data.get(pos[0], pos[1], pos[2]);
}

const ::bbox grid::bbox() const {
	return m_bbox;
}

const grid_data& grid::data() const {
	return m_data;
}

const std::array<float, 3> grid::element_size() const {
	return (m_bbox.max - m_bbox.min) / m_depthPow;
}

void grid::visit_active(const std::function<void (const ::bbox&)>& visitor) {
	m_data.visit_active(visitor, m_bbox);
}
