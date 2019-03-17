#pragma once

#include "geos_type_def.hpp"
#include <unordered_map>
#include <vector>


namespace spatial_mapper {

template<class T>
using cls_filter = std::enable_if_t<
	std::is_base_of<CollectionInput, T>::value>;

class Mapper {
public:
	static constexpr size_t rtree_node_capacity = 10;
	Mapper() :
		global_handle_{ GEOS_init_r() },
		rtree_{ GEOSSTRtree_create_r(hl(), rtree_node_capacity), hl() }
	{}
	template<class T, class = cls_filter<T>>
	Mapper(T const& input) : Mapper() {
		vec_geometry_ = create_geometry_vec<T>(input);
		create_internal_map();
		insert_rtree();
	}
	GEOSContextHandle_t hl() const { return global_handle_.get(); }
	GEOSSTRtree* rt() const { return rtree_.get(); }

	template<class T, class = cls_filter<T>>
	IDVec find_nearest(T const& input) const {
		GeoVec geo_vec_to_map = create_geometry_vec<T>(input);
		Index size = (Index)geo_vec_to_map.size();
		IDVec mapped_ind;
		mapped_ind.reserve(size);
		for (Index i = 0; i < size; i++)
			mapped_ind[i] = find_nearest(geo_vec_to_map[i].get());
		return mapped_ind;
	}
private:
	GlobalHandle const global_handle_;
	RTreeHandle const rtree_;
	GeoVec vec_geometry_;
	std::unordered_map<GEOSGeometry const*, Index> map_geometry_;

	CoordSeqHandle create_coord_seq(PointCoord const* pt, Index n) const;
	GeometryHandle create_point(PointCoord const* pt, Index n) const;
	GeometryHandle create_linestring(PointCoord const* pt, Index n) const;
	GeometryHandle create_polygon(PointCoord const* pt, Index n) const;
	
	template<class T, class = cls_filter<T>>
	GeoVec create_geometry_vec(
			CollectionInput const& input) const;
	void create_internal_map();
	void insert_rtree() const;

	Index find_nearest(GEOSGeometry const* geo) const;
};

}