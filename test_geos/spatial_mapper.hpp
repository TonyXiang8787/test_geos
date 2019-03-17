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
	template<class T, class = std::enable_if_t<
		std::is_base_of<T, CollectionInput>::value>>
	Mapper(T const& input) : Mapper() {

	}

	GEOSContextHandle_t hl() { return global_handle_.get(); }
	GEOSSTRtree* rt() { return rtree_.get(); }
private:
	GlobalHandle const global_handle_;
	RTreeHandle const rtree_;
	std::vector<GeometryHandle> vec_geometry_;
	std::unordered_map<GEOSGeometry const*, Index> map_geometry_;

	CoordSeqHandle create_coord_seq(PointCoord const* pt, Index n);
	GeometryHandle create_point(PointCoord const* pt, Index n);
	GeometryHandle create_linestring(PointCoord const* pt, Index n);
	GeometryHandle create_polygon(PointCoord const* pt, Index n);
	
	template<class T, class = std::enable_if_t<
		std::is_base_of<T, CollectionInput>::value>>
	std::vector<GeometryHandle> create_geometry_vec(CollectionInput const& input) {
		Index size = (Index)input.indptr.size() - 1;
		std::vector<GeometryHandle> vec;
		vec.reserve(size);
		for (Index i = 0; i < size; i++)
		{
			Index begin = input.indptr[i];
			Index len = input.indptr[i + 1] - begin;
			if constexpr (std::is_same<T, PointCollection>::value)
				vec.push_back(create_point(&input.data[begin], len));
			else if constexpr (std::is_same<T, LineStringCollection>::value)
				vec.push_back(create_linestring(&input.data[begin], len));
			else if constexpr (std::is_same<T, PolygonCollection>::value)
				vec.push_back(create_polygon(&input.data[begin], len));
		}
		return vec;
	}
	
	void create_internal_map();
};

}