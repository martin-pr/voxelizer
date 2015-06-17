#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <tbb/task_scheduler_init.h>

#include "mesh.h"
#include "grid.h"
#include "factory.tpp"
#include "array_maths.h"
#include "sampler.h"

#include "../common/config.h"
#include "../common/common.h"

namespace po = boost::program_options;

using std::cout;
using std::endl;
using std::flush;

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

		cout << "Loading file " << path.string() << " ... " << flush;
		std::unique_ptr<mesh> object(factory<mesh>::create(path.string()));
		if(object.get() == NULL)
			throw std::runtime_error("Cannot load file " + path.string());
		cout << "done." << endl;

		// create the grid
		grid g(level, object->bbox());

		// initialise tbb
		tbb::task_scheduler_init init;

		// sample the object and write the result to the grid
		cout << "Sampling... " << flush;

		{
			sampler s(g);
			s.add(*object);
		}

		cout << "done." << endl;

		// and write out the result
		{
			cout << "Writing... " << flush;

			std::ofstream ofs(cache_path(path).string().c_str());
			boost::archive::binary_oarchive oa(ofs);
			oa << g;

			cout << "done." << endl;
		}
	}
}
