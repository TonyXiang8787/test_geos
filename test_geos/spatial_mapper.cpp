#include "spatial_mapper.hpp"
#include "exception.hpp"

namespace spatial_mapper {

CoordSeqHandle Mapper::create_coord_seq(PointCoord const* pt, Index n)
{
	GEOSCoordSequence* coord_seq = GEOSCoordSeq_create_r(hl(), n, 2);
	if (!coord_seq)
		THROW(PointSeqError);
	CoordSeqHandle coord_seq_hl{ coord_seq, hl() };
	for (Index i = 0; i < n; i++) {
		int intx = GEOSCoordSeq_setX_r(hl(), coord_seq, i, pt[i].x);
		int inty = GEOSCoordSeq_setY_r(hl(), coord_seq, i, pt[i].y);
		if (intx * inty == 0)
			THROW(PointSeqError);
	}
	return coord_seq_hl;
}

GeometryHandle Mapper::create_point(PointCoord const* pt, Index)
{
	CoordSeqHandle seq = create_coord_seq(pt, 1);
	GEOSGeometry* geo = GEOSGeom_createPoint_r(hl(), seq.get());
	if (!geo)
		THROW(PointError);
	GeometryHandle geo_hl{ geo, hl() };
	seq.release(); 
	return geo_hl;
}

GeometryHandle Mapper::create_linestring(PointCoord const* pt, Index n)
{
	CoordSeqHandle seq = create_coord_seq(pt, n);
	GEOSGeometry* geo = GEOSGeom_createLineString_r(hl(), seq.get());
	if (!geo)
		THROW(LineStringError);
	GeometryHandle geo_hl{ geo, hl() };
	seq.release();
	return geo_hl;
}

GeometryHandle Mapper::create_polygon(PointCoord const* pt, Index n)
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

void Mapper::create_internal_map()
{
	Index const size = (Index)vec_geometry_.size();
	for (Index i = 0; i < size; i++)
		map_geometry_[vec_geometry_[i].get()] = i;
}

}