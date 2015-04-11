#include <fstream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "array_maths.h"
#include "factory.tpp"
#include "mesh.h"

namespace {

factory<mesh> s_objFactory(".obj", [](boost::filesystem::path p) {
	std::unique_ptr<mesh> result(new mesh());

	std::ifstream file(p.string().c_str());

	std::string line;
	while(std::getline(file, line))
		if(line.length() > 2) {
			if((line[0] == 'v') && (line[1] == ' ')) {
				std::stringstream v(line.substr(1));

				std::array<float, 3> vertex;
				v >> vertex[0] >> vertex[1] >> vertex[2];
				result->add_vertex(vertex);
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
						result->add_face(val);
				}
			}
		}

	return std::move(result);
});

}
