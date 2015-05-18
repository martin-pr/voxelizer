#include <iostream>
#include <string>
#include <stdexcept>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "mesh.h"
#include "grid.h"
#include "factory.tpp"

namespace po = boost::program_options;

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("input", po::value<std::string>(), "input file (alembic or obj)")
		("level", po::value<unsigned>(), "grid level (defaults to 7)")
	;

	// process the options
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help") || (!vm.count("input"))) {
		cout << desc << "\n";
		return 1;
	}

	unsigned level = 7;
	if(vm.count("level"))
		level = vm["level"].as<unsigned>();

	if(vm.count("input")) {
		// load the input file
		const boost::filesystem::path path = vm["input"].as<std::string>();
		if(!boost::filesystem::exists(path))
			throw std::runtime_error("cannot find input file " + path.string());

		std::unique_ptr<mesh> object(factory<mesh>::create(argv[1]));
		if(object.get() == NULL)
			throw std::runtime_error("Cannot load file " + path.string());

		// create the grid
		grid g(level, object->bbox());

		// sample the object and write the result to the grid
		auto elem = g.element_size();
		const float minSample = std::min(std::min(elem[0], elem[1]), elem[2]) / 2.0f;

		for(const auto& v : object->sample(minSample))
			g.set(v);
	}
}
