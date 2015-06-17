#include "sampler.h"

#include <mutex>

#include <tbb/tbb.h>

#include "array_maths.h"

sampler::sampler(grid& g) : m_grid(g) {
}

void sampler::sampleTriangle(const std::array<float, 3>& v1, const std::array<float, 3>& v2, const std::array<float, 3>& v3, const float maxEdgeLenSquared) {
	static std::mutex mutex;

	const float sql1 = squaredLength(v1-v2);
	const float sql2 = squaredLength(v1-v3);
	const float sql3 = squaredLength(v2-v3);

	if((sql1 < maxEdgeLenSquared) && (sql2 < maxEdgeLenSquared) && (sql3 < maxEdgeLenSquared)) {
		std::unique_lock<std::mutex> lock(mutex);

		m_grid.set(v1);
		m_grid.set(v2);
		m_grid.set(v3);
	}

	else if((sql1 > sql2) && (sql1 > sql3)) {
		const std::array<float, 3> mid = (v1+v2) / 2.0f;
		sampleTriangle(v1, mid, v3, maxEdgeLenSquared);
		sampleTriangle(mid, v2, v3, maxEdgeLenSquared);
	}
	else if((sql2 > sql1) && (sql2 > sql3)) {
		const std::array<float, 3> mid = (v1+v3) / 2.0f;
		sampleTriangle(v3, mid, v2, maxEdgeLenSquared);
		sampleTriangle(mid, v1, v2, maxEdgeLenSquared);
	}
	else {
		const std::array<float, 3> mid = (v2+v3) / 2.0f;
		sampleTriangle(v1, mid, v3, maxEdgeLenSquared);
		sampleTriangle(mid, v1, v2, maxEdgeLenSquared);
	}
}

void sampler::add(const mesh& m) {
	auto elem = m_grid.element_size();
	const float minSample = std::min(std::min(elem[0], elem[1]), elem[2]) / 2.0f;

	tbb::parallel_for((size_t)0, m.faces().size(), (size_t)1, [&](size_t i) {
		auto& face = m.faces()[i];
		sampleTriangle(m.vertices()[face[0]], m.vertices()[face[1]], m.vertices()[face[2]], minSample*minSample);
	});
}
