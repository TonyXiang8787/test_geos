// test_geos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "spatial_mapper.hpp"

int main()
{
	spatial_mapper::PolygonCollection p;
	p.indptr = { 0, 4 };
	p.data = { { 0.0, 0.0 }, { 2.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 0.0 } };
	spatial_mapper::Mapper mapper{ p };

}
