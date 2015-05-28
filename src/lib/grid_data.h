#pragma once

#include <array>
#include <memory>

#include <boost/optional.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "bbox.h"

class grid_data {
	public:
		grid_data(unsigned char level);

		grid_data(const grid_data& d);
		grid_data& operator = (const grid_data& d);

		void set(unsigned x, unsigned y, unsigned z);
		const bool get(unsigned x, unsigned y, unsigned z) const;

		const unsigned char level() const;

		void visit_active(const std::function<void (const ::bbox&)>& visitor, const ::bbox& parent_bbox, unsigned max_level);

	protected:
	private:
		unsigned char m_level;
		unsigned m_mask;
		std::array<boost::shared_ptr<grid_data>, 8> m_data;

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & m_level;
		ar & m_mask;
		ar & m_data;
	}

};
