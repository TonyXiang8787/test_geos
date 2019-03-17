#pragma once

#include <exception>

namespace spatial_mapper {

struct MapperError : public std::exception
{
	const char * what() const noexcept
	{
		return "Spatial Mapper Error!";
	}
};


struct PointSeqError : MapperError {};

struct PointError : MapperError {};

struct LineStringError : MapperError {};

struct PolygonError : MapperError {};


}