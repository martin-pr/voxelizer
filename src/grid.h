#pragma once

#include <array>
#include <functional>

#include "grid_data.h"
#include "bbox.h"

class grid_iterator;

class grid {
	public:
		grid(unsigned char depth, const ::bbox& box);

		void set(const std::array<float, 3>& pos);
		void set(const std::array<unsigned, 3>& pos);
		const bool get(const std::array<float, 3>& pos) const;
		const bool get(const std::array<unsigned, 3>& pos) const;

		const ::bbox bbox() const;

		/// direct read-only data access, to be replaced by iterators
		const grid_data& data() const;

		/// returns the element box size
		const std::array<float, 3> element_size() const;

		/// a silly way of visiting all elements - to be replaced, eventually, with something more
		///   like an iterator (and more efficient)
		void visit_active(const std::function<void (const ::bbox&)>& visitor);

	protected:
	private:
		const std::array<unsigned, 3> coord(const std::array<float, 3>& pos) const;

		float m_depthPow;
		unsigned m_coordLimit;
		::bbox m_bbox;

		grid_data m_data;

	friend class grid_iterator;
};
