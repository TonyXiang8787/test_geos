#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace spatial_mapper {

struct MapperError : public std::exception
{
	MapperError(char const* file, int line) :
		f{file}, l{line}
	{ }

	const char * what() const noexcept
	{
		std::stringstream  o;
		o << "Error at \n file: " << f << '\n' << "line: " << l << "\n";
		o << msg();
		return o.str().c_str();
	}
	virtual std::string msg() const { return "Spatial Mapper Error!"; }
	char const* f;
	int l;
};


#define SUBCLSEX(CLS, MSG) \
struct CLS : MapperError { \
	CLS(char const* file, int line) : MapperError(file, line) {} \
	std::string msg() const override { return MSG; } \
}

SUBCLSEX(PointSeqError, "Point sequence error!");
SUBCLSEX(PointError, "Point error!");
SUBCLSEX(LineStringError, "Line string error!");
SUBCLSEX(PolygonError, "Polygon error!");
SUBCLSEX(RTreeError, "RTree error!");

#undef SUBCLSEX

}