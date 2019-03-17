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

	spatial_mapper::PointCollection pp;
	pp.indptr = { 0, 1 };
	pp.data = { { 0.5, 0.5 } };
	auto x = mapper.find_nearest(pp);
	std::cout << x[0] << '\n';
}
