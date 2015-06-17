#pragma once

#include <vector>
#include <functional>

#include "bbox.h"

class mesh {
	public:
		mesh();
		virtual ~mesh();

		void add_vertex(const std::array<float, 3>& v);
		void add_face(const std::array<unsigned, 3>& f);

		const std::vector<std::array<float, 3>>& vertices() const;
		const std::vector<std::array<unsigned, 3>>& faces() const;

		const ::bbox bbox() const;

		void normalize();

	private:
		std::vector<std::array<float, 3>> m_vertices;
		std::vector<std::array<unsigned, 3>> m_faces;

		::bbox m_bbox;
};
