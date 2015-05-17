#pragma once

#include <memory>
#include <functional>
#include <boost/filesystem/path.hpp>

template<typename T>
class factory {
	public:
		static std::unique_ptr<T> create(const boost::filesystem::path& file);

		factory(const std::string& extension, std::function<std::unique_ptr<T>(boost::filesystem::path)> factoryMethod);
		~factory();

	private:
		std::unique_ptr<T> factoryMethod(boost::filesystem::path p);

		std::function<std::unique_ptr<T>(boost::filesystem::path)> m_factoryMethod;
};
