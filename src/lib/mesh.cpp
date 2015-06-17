#include "mesh.h"

#include <fstream>
#include <sstream>
#include <cassert>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "array_maths.h"

mesh::mesh() : m_bbox({{0,0,0}}, {{0,0,0}}) {
}

mesh::~mesh() {
}

void mesh::add_vertex(const std::array<float, 3>& v) {
	// add the vertex
	m_vertices.push_back(v);

	// and update the bbox
	if(m_vertices.size() == 1) {
		m_bbox.min = m_vertices.back();
		m_bbox.max = m_vertices.back();
	}
	else
		for(unsigned a=0; a<3; ++a) {
			m_bbox.min[a] = std::min(m_bbox.min[a], v[a]);
			m_bbox.max[a] = std::max(m_bbox.max[a], v[a]);
		}
}

void mesh::add_face(const std::array<unsigned, 3>& f) {
	assert((f[0] < m_vertices.size()) && (f[1] < m_vertices.size()) && (f[2] < m_vertices.size()));

	m_faces.push_back(f);
}

const std::vector<std::array<float, 3>>& mesh::vertices() const {
	return m_vertices;
}

const std::vector<std::array<unsigned, 3>>& mesh::faces() const {
	return m_faces;
}

const ::bbox mesh::bbox() const {
	return m_bbox;
}

void mesh::normalize() {
	// centering
	std::array<float, 3> mid = (m_bbox.max + m_bbox.min) / 2.0f;
	mid[1] = m_bbox.min[1];

	// maximum size on any of the axes
	const float size = std::max(std::max(m_bbox.max[0] - m_bbox.min[0], m_bbox.max[1] - m_bbox.min[1]), m_bbox.max[2] - m_bbox.min[2]);

	// normalize each vertex
	for(auto& v : m_vertices)
		v = (v - mid) / size;

	// and set a new bounding box
	const std::array<float, 3> half = (m_bbox.max - m_bbox.min) / 2.0f / size;
	m_bbox = ::bbox({{-half[0], 0.0f, -half[2]}}, {{half[0], 2.0f*half[1], half[2]}});
}
