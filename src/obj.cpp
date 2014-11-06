#include "obj.h"

#include <fstream>
#include <sstream>
#include <cassert>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "array_maths.h"

obj::obj() {
}

obj::obj(const boost::filesystem::path& path) {
	std::ifstream file(path.string().c_str());

	std::string line;
	while(std::getline(file, line))
		if(line.length() > 2) {
			if((line[0] == 'v') && (line[1] == ' ')) {
				std::stringstream v(line.substr(1));

				m_vertices.push_back(std::array<float, 3>());
				v >> m_vertices.back()[0] >> m_vertices.back()[1] >> m_vertices.back()[2];

				if(m_vertices.size() == 1) {
					m_min = m_vertices.back();
					m_max = m_vertices.back();
				}
				else
					for(unsigned char c=0;c<3;++c) {
						m_min[c] = std::min(m_vertices.back()[c], m_min[c]);
						m_max[c] = std::max(m_vertices.back()[c], m_max[c]);
					}
			}

			if(line[0] == 'f') {
				line = line.substr(1);
				boost::algorithm::trim(line);

				std::vector<std::string> face;
				boost::algorithm::split(face, line, boost::is_any_of(" "));

				// parsing the face vertices with trivial triangulation
				std::array<unsigned, 3> val;
				for(unsigned a=0;a<face.size();++a) {
					std::stringstream s(face[a]);
					s >> val[std::min(2u,a)];
					val[std::min(2u,a)] -= 1;
					if(a >= 2)
						m_faces.push_back(val);
				}
			}
		}
}

const std::vector<std::array<float, 3>>& obj::vertices() const {
	return m_vertices;
}

const std::vector<std::array<unsigned, 3>>& obj::faces() const {
	return m_faces;
}

const std::pair<std::array<float, 3>, std::array<float, 3>> obj::bbox() const {
	return std::make_pair(m_min, m_max);
}

void obj::normalize() {
	// centering
	std::array<float, 3> mid = (m_max + m_min) / 2.0f;
	mid[1] = m_min[1];

	// maximum size on any of the axes
	const float size = std::max(std::max(m_max[0] - m_min[0], m_max[1] - m_min[1]), m_max[2] - m_min[2]);

	// and normalize each vertex
	for(auto& v : m_vertices)
		v = (v - mid) / size;
}
