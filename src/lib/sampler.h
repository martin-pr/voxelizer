#pragma once

#include <array>
#include <functional>

#include "grid.h"
#include "mesh.h"

class sampler {
	public:
		sampler(grid& g);

		void add(const mesh& m);

	protected:
	private:
		void sampleTriangle(const std::array<float, 3>& v1, const std::array<float, 3>& v2, const std::array<float, 3>& v3, const float maxEdgeLenSquared);

		grid& m_grid;
};
