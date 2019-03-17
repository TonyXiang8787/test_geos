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
	pp.indptr = { 0, 1, 2, 3 };
	pp.data = { { 1.0, 0.5 }, { 1.9, 0.9 }, { 2.5, 2.5 } };
	auto x = mapper.find_nearest(pp);
	std::cout << x[0] << '\n';
	std::cout << x[1] << '\n';
	std::cout << x[2] << '\n';
	auto y = mapper.find_intersect(pp);
}
