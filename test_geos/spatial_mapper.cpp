#include "spatial_mapper.hpp"
#include "exception.hpp"

namespace spatial_mapper {

GEOSContextHandle_t Mapper::create_geos_handle()
{
	GEOSContextHandle_t hl = GEOS_init_r();
	if (!hl)
		THROW(MapperError);
	return hl;
}

GEOSSTRtree* Mapper::create_rtree_handle() const
{
	GEOSSTRtree* rt = GEOSSTRtree_create_r(hl(), rtree_node_capacity);
	if (!rt)
		THROW(RTreeError);
	return rt;
}


CoordSeqHandle Mapper::create_coord_seq(PointCoord const* pt, Index n) const
{
	GEOSCoordSequence* coord_seq = GEOSCoordSeq_create_r(hl(), n, 2);
	if (!coord_seq)
		THROW(PointSeqError);
	CoordSeqHandle coord_seq_hl{ coord_seq, hl() };
	for (Index i = 0; i < n; i++) {
		int error_x = GEOSCoordSeq_setX_r(hl(), coord_seq, i, pt[i].x);
		int error_y = GEOSCoordSeq_setY_r(hl(), coord_seq, i, pt[i].y);
		if (error_x * error_y == 0)
			THROW(PointSeqError);
	}
	return coord_seq_hl;
}

GeometryHandle Mapper::create_point(PointCoord const* pt, Index) const
{
	CoordSeqHandle seq = create_coord_seq(pt, 1);
	GEOSGeometry* geo = GEOSGeom_createPoint_r(hl(), seq.get());
	if (!geo)
		THROW(PointError);
	GeometryHandle geo_hl{ geo, hl() };
	seq.release(); 
	return geo_hl;
}

GeometryHandle Mapper::create_linestring(PointCoord const* pt, Index n) const
{
	CoordSeqHandle seq = create_coord_seq(pt, n);
	GEOSGeometry* geo = GEOSGeom_createLineString_r(hl(), seq.get());
	if (!geo)
		THROW(LineStringError);
	GeometryHandle geo_hl{ geo, hl() };
	seq.release();
	return geo_hl;
}

GeometryHandle Mapper::create_polygon(PointCoord const* pt, Index n) const
{
	CoordSeqHandle seq = create_coord_seq(pt, n);
	// linear ring
	GEOSGeometry* geo_ring = GEOSGeom_createLinearRing_r(hl(), seq.get());
	if (!geo_ring)
		THROW(PolygonError);
	GeometryHandle geo_ring_hl{ geo_ring, hl() };
	seq.release();
	// polygon
	GEOSGeometry* geo = GEOSGeom_createPolygon_r(hl(), geo_ring_hl.get(), nullptr, 0);
	if (!geo)
		THROW(PolygonError);
	GeometryHandle geo_hl{ geo, hl() };
	geo_ring_hl.release();
	return geo_hl;
}

std::unordered_map<GEOSGeometry const*, Index> 
Mapper::create_internal_map() const
{
	Index const size = (Index)vec_geometry_.size();
	std::unordered_map<GEOSGeometry const*, Index> map_geometry;
	for (Index i = 0; i < size; i++)
		map_geometry[vec_geometry_[i].get()] = i;
	return map_geometry;
}

void Mapper::insert_rtree() const
{
	for (GeometryHandle const& geo : vec_geometry_)
		GEOSSTRtree_insert_r(hl(), rt(), geo.get(), geo.get());
}

Index Mapper::find_nearest(GEOSGeometry* geo) const
{
	GEOSGeometry const *nearest = GEOSSTRtree_nearest_r(hl(), rt(), geo);
	if (!nearest)
		THROW(RTreeError);
	return map_geometry_.at(nearest);
}

template<class T, class>
GeoVec Mapper::create_geometry_vec(
		CollectionInput const& input) const {
	Index size = (Index)input.indptr.size() - 1;
	GeoVec vec;
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
template std::vector<GeometryHandle> Mapper::create_geometry_vec
<PointCollection>(CollectionInput const& input) const;
template std::vector<GeometryHandle> Mapper::create_geometry_vec
<LineStringCollection>(CollectionInput const& input) const;
template std::vector<GeometryHandle> Mapper::create_geometry_vec
<PolygonCollection>(CollectionInput const& input) const;


IDVec Mapper::find_intersect(GEOSGeometry* geo) const
{
	CallBackData call_back_data{ hl(), geo, {}, 0 };
	GEOSSTRtree_query_r(hl(), rt(), geo, call_back_intersect, &call_back_data);
	if (call_back_data.error_code == 1)
		THROW(RTreeError);
	IDVec id_vec;
	id_vec.reserve(call_back_data.mapped_ptrs.size());
	for (GEOSGeometry const* ptr : call_back_data.mapped_ptrs)
		id_vec.push_back(map_geometry_.at(ptr));
	return id_vec;
}

}