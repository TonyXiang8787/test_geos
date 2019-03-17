#pragma once

#include "geos_type_def.hpp"
#include <unordered_map>
#include <vector>


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
	std::vector<GeometryHandle> vec_geometry_;
	std::unordered_map<GEOSGeometry*, Index> map_geometry_;

	CoordSeqHandle create_coord_seq(PointCoord const* pt, Index n);
	GeometryHandle create_point(PointCoord const* pt, Index n);
	GeometryHandle create_linestring(PointCoord const* pt, Index n);
	GeometryHandle create_polygon(PointCoord const* pt, Index n);

	void create_internal_map();
};

}