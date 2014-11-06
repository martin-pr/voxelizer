#pragma once

#include <vector>

#include <boost/filesystem.hpp>

class obj {
	public:
		obj();
		obj(const boost::filesystem::path& path);

		const std::vector<std::array<float, 3>>& vertices() const;
		const std::vector<std::array<unsigned, 3>>& faces() const;

		const std::pair<std::array<float, 3>, std::array<float, 3>> bbox() const;

		void normalize();

	protected:
	private:
		std::vector<std::array<float, 3>> m_vertices;
		std::vector<std::array<unsigned, 3>> m_faces;

		std::array<float, 3> m_min, m_max;
};
