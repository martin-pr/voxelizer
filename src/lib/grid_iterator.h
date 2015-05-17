#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include "grid.h"

struct grid_cell {
	std::array<float, 3> min, max;
};

class grid_iterator : public boost::iterator_facade<grid_iterator, grid_cell, boost::forward_traversal_tag> {
	public:
		grid_iterator(grid& g, bool endIterator = false);

	protected:
	private:
		struct iterator_item {
			std::array<std::shared_ptr<grid_data> current, end;
		};

		std::vector<iterator_item> m_items;
};
