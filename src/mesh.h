#pragma once

#include <vector>

#include "bbox.h"

class mesh {
	public:
		mesh();
		virtual ~mesh();

		const std::vector<std::array<float, 3>>& vertices() const;
		const std::vector<std::array<unsigned, 3>>& faces() const;

		const ::bbox bbox() const;

		void normalize();

		std::vector<std::array<float, 3>> sample(float maxEdgeLen);

	protected:
		void add_vertex(const std::array<float, 3>& v);
		void add_face(const std::array<unsigned, 3>& f);

	private:
		std::vector<std::array<float, 3>> m_vertices;
		std::vector<std::array<unsigned, 3>> m_faces;

		::bbox m_bbox;
};
