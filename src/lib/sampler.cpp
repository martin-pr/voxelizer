#include "sampler.h"

#include <mutex>
#include <stack>

#include <tbb/tbb.h>

#include "array_maths.h"

sampler::sampler(grid& g) : m_grid(g) {
}

void sampler::sampleTriangle(std::array<float, 3> v1, std::array<float, 3> v2, std::array<float, 3> v3, const float maxEdgeLenSquared) {
	static std::mutex mutex;

	std::stack<std::array<std::array<float,3>,3>> stack;
	stack.push(std::array<std::array<float,3>,3>{v1,v2,v3});

	while(!stack.empty()) {
		v1 = stack.top()[0];
		v2 = stack.top()[1];
		v3 = stack.top()[2];
		stack.pop();

		const auto c1 = m_grid.coord(v1);
		const auto c2 = m_grid.coord(v2);
		const auto c3 = m_grid.coord(v3);

		const float sql1 = squaredLength(v1-v2);
		const float sql2 = squaredLength(v1-v3);
		const float sql3 = squaredLength(v2-v3);

		if(((c1 == c2) && (c1 == c3)) || ((sql1 < maxEdgeLenSquared) && (sql2 < maxEdgeLenSquared) && (sql3 < maxEdgeLenSquared))) {
			std::unique_lock<std::mutex> lock(mutex);

			m_grid.set(v1);
			m_grid.set(v2);
			m_grid.set(v3);
		}

		else {
			if((sql1 > sql2) && (sql1 > sql3)) {
				const std::array<float, 3> mid = (v1+v2) / 2.0f;
				stack.push({v1, mid, v3});
				stack.push({mid, v2, v3});
			}
			else if((sql2 > sql1) && (sql2 > sql3)) {
				const std::array<float, 3> mid = (v1+v3) / 2.0f;
				stack.push({v3, mid, v2});
				stack.push({mid, v1, v2});
			}
			else {
				const std::array<float, 3> mid = (v2+v3) / 2.0f;
				stack.push({v1, mid, v3});
				stack.push({mid, v1, v2});
			}
		}
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
