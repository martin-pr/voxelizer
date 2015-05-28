#include "config.h"

namespace {
	static std::unique_ptr<config> s_singleton;
}

#include <boost/property_tree/ini_parser.hpp>

void config::init(const boost::filesystem::path& path) {
	boost::property_tree::ptree data;
	boost::property_tree::ini_parser::read_ini(path.string(), data);

	init(data);
}

void config::init(const boost::property_tree::ptree& values) {
	s_singleton = std::unique_ptr<config>(new config(values));
}

config& config::singleton() {
	static const boost::filesystem::path s_defaultConfig = "./config.ini";
	if(s_singleton.get() == NULL)
		init(s_defaultConfig);
	assert(s_singleton.get() != NULL);

	return *s_singleton;
}

config::config(const boost::property_tree::ptree& values) : m_values(values) {
}

