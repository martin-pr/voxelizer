#pragma once

#include <cassert>
#include <boost/bimap.hpp>

#include "factory.h"

template<typename T>
boost::bimap<std::string, factory<T>*>& factories() {
	static std::unique_ptr< boost::bimap<std::string, factory<T>*> > s_instance;
	if(s_instance.get() == NULL)
		s_instance = std::move(std::unique_ptr< boost::bimap<std::string, factory<T>*> >(new boost::bimap<std::string, factory<T>*>()));

	return *s_instance;
}

template<typename T>
factory<T>::factory(const std::string& extension, std::function<std::unique_ptr<T>(boost::filesystem::path)> factoryMethod) :
	m_factoryMethod(factoryMethod) {

	assert(factories<T>().left.find(extension) == factories<T>().left.end());

	factories<T>().left.insert(std::make_pair(extension, this));
}

template<typename T>
factory<T>::~factory() {
	auto it = factories<T>().right.find(this);
	if(it != factories<T>().right.end())
		factories<T>().right.erase(it);
}

template<typename T>
std::unique_ptr<T> factory<T>::factoryMethod(boost::filesystem::path p) {
	return std::move(m_factoryMethod(p));
}

template<typename T>
std::unique_ptr<T> factory<T>::create(const boost::filesystem::path& file) {
	auto it = factories<T>().left.find(file.extension().string());
	if(it != factories<T>().left.end())
		return it->second->factoryMethod(file);

	return std::unique_ptr<T>();
}
