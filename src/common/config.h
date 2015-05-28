#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

class config {
	public:
		static void init(const boost::filesystem::path& path);
		static void init(const boost::property_tree::ptree& values);

		template<typename T>
		static const T get(const std::string& key) {
			return singleton().m_values.get<T>(key);
		}

	private:
		config(const boost::property_tree::ptree& values);

		static config& singleton();

		boost::property_tree::ptree m_values;
};
