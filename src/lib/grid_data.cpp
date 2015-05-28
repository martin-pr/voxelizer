#include "grid_data.h"

#include <array>
#include <cassert>

#include "array_maths.h"

namespace {
	// static "leaf" data node - to be used as a shared instance for the leafest (0 level) nodes
	static boost::shared_ptr<grid_data> s_leafData(new grid_data(0));

	inline const unsigned coord(unsigned x, unsigned y, unsigned z, unsigned mask) {
		return (x & mask ? 4 : 0) + (y & mask ? 2 : 0) + (z & mask ? 1 : 0);
	}
}

grid_data::grid_data(unsigned char level) : m_level(level), m_mask(1 << level) {
}

grid_data::grid_data(const grid_data& d) : m_level(d.m_level), m_mask(d.m_mask) {
	for(unsigned i=0; i<8; ++i)
		if(d.m_data[i].get() != NULL) {
			if(m_level > 0)
				m_data[i] = boost::shared_ptr<grid_data>(new grid_data(*d.m_data[i]));
			else
				m_data[i] = s_leafData;
		}
}

grid_data& grid_data::operator = (const grid_data& d) {
	m_level = d.m_level;
	m_mask = d.m_mask;

	for(unsigned i=0; i<8; ++i)
		if(d.m_data[i].get() != NULL) {
			if(m_level > 0)
				m_data[i] = boost::shared_ptr<grid_data>(new grid_data(*d.m_data[i]));
			else
				m_data[i] = s_leafData;
		}

	return *this;
}

void grid_data::set(unsigned x, unsigned y, unsigned z) {
	// get the coordinate (binary 000...xyz) = a number < 8
	const unsigned c = coord(x,y,z,m_mask);
	assert(c < 8);

	// set the value
	if(m_level == 0) {
		m_data[c] = s_leafData;
	}
	else {
		if(m_data[c].get() == NULL)
			m_data[c] = boost::shared_ptr<grid_data>(new grid_data(m_level-1));
		m_data[c]->set(x,y,z);
	}
}

const bool grid_data::get(unsigned x, unsigned y, unsigned z) const {
	const unsigned c = coord(x,y,z,m_mask);
	assert(c < 8);

	if(m_data[c].get() == NULL)
		return false;

	// on 0th level, anything non-null (s_leafData, to be precise) means true,
	//   on other levels, we have to go on recursively
	return(m_level == 0) || (m_data[c]->get(x,y,z));
}

const unsigned char grid_data::level() const {
	return m_level;
}

void grid_data::visit_active(const std::function<void (const ::bbox&)>& visitor, const ::bbox& parent_bbox, unsigned max_level) {
	const std::array<float, 3> half_size = (parent_bbox.max - parent_bbox.min) / 2.0f;

	// for all children
	for(unsigned char a=0;a<8;++a)
		// if the child is non-null
		if(m_data[a].get() != NULL) {
			// compute the origin of the bounding box
			const std::array<float, 3> min = {{
				parent_bbox.min[0] + (a&4 ? half_size[0] : 0.0f),
				parent_bbox.min[1] + (a&2 ? half_size[1] : 0.0f),
				parent_bbox.min[2] + (a&1 ? half_size[2] : 0.0f),
			}};

			// if this is the leaf level, return the computed 1/8th bbox
			if((m_level == 0) || (max_level == 0))
				visitor(::bbox(min, min+half_size));
			// otherwise continue recursively
			else
				m_data[a]->visit_active(visitor, ::bbox(min, min+half_size), max_level-1);
		}
}
