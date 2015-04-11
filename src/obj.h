#pragma once

#include <vector>

#include <boost/filesystem.hpp>

#include "mesh.h"

class obj : public mesh {
	public:
		obj(const boost::filesystem::path& path);
};
