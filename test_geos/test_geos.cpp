// test_geos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "spatial_mapper.hpp"

int main()
{
	spatial_mapper::Mapper mapper{};
	GEOSContextHandle_t hl = mapper.hl();
	GEOSCoordSequence* coord_seq = GEOSCoordSeq_create_r(hl, 4, 2);
	GEOSCoordSeq_setX_r(hl, coord_seq, 0, 0.0);
	GEOSCoordSeq_setY_r(hl, coord_seq, 0, 0.0);
	GEOSCoordSeq_setX_r(hl, coord_seq, 1, 2.0);
	GEOSCoordSeq_setY_r(hl, coord_seq, 1, 0.0);
	GEOSCoordSeq_setX_r(hl, coord_seq, 2, 1.0);
	GEOSCoordSeq_setY_r(hl, coord_seq, 2, 1.0);
	GEOSCoordSeq_setX_r(hl, coord_seq, 3, 0.0);
	GEOSCoordSeq_setY_r(hl, coord_seq, 3, 0.0);

	GEOSGeometry* linear_ring = GEOSGeom_createLinearRing_r(hl, coord_seq);
	spatial_mapper::GeometryHandle polygon{ GEOSGeom_createPolygon_r(hl, linear_ring, nullptr, 0), hl };
}
