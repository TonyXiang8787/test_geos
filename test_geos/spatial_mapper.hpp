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

	template<class T, class = cls_filter<T>>
	Mapper(T const& input) : 
		geos_handle_{ create_geos_handle() },
		rtree_{ create_rtree_handle(), hl() },
		vec_geometry_{ create_geometry_vec<T>(input) },
		map_geometry_{ create_internal_map() }
	{
		insert_rtree();
	}
	GEOSContextHandle_t hl() const { return geos_handle_.get(); }
	GEOSSTRtree* rt() const { return rtree_.get(); }

	template<class T, class = cls_filter<T>>
	IDVec find_nearest(T const& input) const {
		GeoVec geo_vec_to_map = create_geometry_vec<T>(input);
		Index size = (Index)geo_vec_to_map.size();
		IDVec mapped_ind;
		mapped_ind.reserve(size);
		for (GeometryHandle const & geo : geo_vec_to_map)
			mapped_ind.push_back(find_nearest(geo.get()));
		return mapped_ind;
	}

	template<class T, class = cls_filter<T>>
	CollectionOutput find_intersect(T const& input) const {
		GeoVec geo_vec_to_map = create_geometry_vec<T>(input);
		Index size = (Index)geo_vec_to_map.size();
		CollectionOutput mapped;
		mapped.indptr.resize(size + 1);
		mapped.indptr[0] = 0;

		for (Index i = 0; i < size; i++) {
			GEOSGeometry* geo = geo_vec_to_map[i].get();
			IDVec single_map = find_intersect(geo);
			mapped.indptr[i + 1] = mapped.indptr[i] + (Index)single_map.size();
			mapped.data.insert(mapped.data.end(), single_map.begin(), single_map.end());
		}
		return mapped;
	}
private:
	GlobalHandle const geos_handle_;
	RTreeHandle const rtree_;
	GeoVec const vec_geometry_;
	std::unordered_map<GEOSGeometry const*, Index> const map_geometry_;

	// create handle
	static GEOSContextHandle_t create_geos_handle();
	GEOSSTRtree* create_rtree_handle() const;

	// create geometry
	CoordSeqHandle create_coord_seq(PointCoord const* pt, Index n) const;
	GeometryHandle create_point(PointCoord const* pt, Index n) const;
	GeometryHandle create_linestring(PointCoord const* pt, Index n) const;
	GeometryHandle create_polygon(PointCoord const* pt, Index n) const;
	
	template<class T, class = cls_filter<T>>
	GeoVec create_geometry_vec(
			CollectionInput const& input) const;
	std::unordered_map<GEOSGeometry const*, Index> 
		create_internal_map() const;
	void insert_rtree() const;

	Index find_nearest(GEOSGeometry* geo) const;
	IDVec find_intersect(GEOSGeometry* geo) const;
};

}