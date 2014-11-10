#pragma once

#include <vector>

#include <boost/filesystem.hpp>

#include "bbox.h"

class obj {
	public:
		obj();
		obj(const boost::filesystem::path& path);

		const std::vector<std::array<float, 3>>& vertices() const;
		const std::vector<std::array<unsigned, 3>>& faces() const;

		const ::bbox bbox() const;

		void normalize();

	protected:
	private:
		std::vector<std::array<float, 3>> m_vertices;
		std::vector<std::array<unsigned, 3>> m_faces;

		::bbox m_bbox;
};
