#pragma once

#include "geos_type_def.hpp"


namespace spatial_mapper {

class Mapper {
public:
	static constexpr size_t rtree_node_capacity = 10;
	Mapper() :
		global_handle_{ GEOS_init_r() },
		rtree_{ GEOSSTRtree_create_r(hl(), rtree_node_capacity), hl() }
	{}
	GEOSContextHandle_t hl() { return global_handle_.get(); }
	GEOSSTRtree* rt() { return rtree_.get(); }
private:
	GlobalHandle const global_handle_;
	RTreeHandle const rtree_;
};

}