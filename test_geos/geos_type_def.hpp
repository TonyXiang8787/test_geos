#pragma once

#define GEOS_USE_ONLY_R_API
#include "geos_c.h"

#include <memory>
#include <type_traits>
#include <vector>


namespace spatial_mapper {

using Index = uint32_t;

// global
template <auto fn>
using deleter_from_fn = std::integral_constant<
	std::decay_t<decltype(fn)>, fn>;
template <typename T, auto fn>
using unique_ptr_deleter = std::unique_ptr<T, deleter_from_fn<fn>>;
using GlobalHandleObj = std::remove_pointer_t<GEOSContextHandle_t>;
using GlobalHandle = unique_ptr_deleter<GlobalHandleObj, GEOS_finish_r>;

// handle deleter
template <class T, void (*fn)(GEOSContextHandle_t, T*)>
class handle_deleter {
public:
	handle_deleter(GEOSContextHandle_t handle) : hl_{ handle } {}
	void operator()(T* ptr) { fn(hl_, ptr); }
private:
	GEOSContextHandle_t const hl_;
};
template <typename T, void(*fn)(GEOSContextHandle_t, T*)>
using unique_ptr_handle_deleter = std::unique_ptr<T, handle_deleter<T, fn>>;
using RTreeHandle = unique_ptr_handle_deleter<
	GEOSSTRtree, GEOSSTRtree_destroy_r>;
using CoordSeqHandle = unique_ptr_handle_deleter<
	GEOSCoordSequence, GEOSCoordSeq_destroy_r>;
using GeometryHandle = unique_ptr_handle_deleter<
	GEOSGeometry, GEOSGeom_destroy_r>;
using GeoVec = std::vector<GeometryHandle>;

// input data structure
struct PointCoord {
	double x;
	double y;
};
using CoordVec = std::vector<PointCoord>;
using IDVec = std::vector<Index>;
struct CollectionInput {
	CoordVec data;
	IDVec indptr;
};
struct PointCollection : CollectionInput {};
struct LineStringCollection : CollectionInput {};
struct PolygonCollection : CollectionInput {};
static_assert(sizeof(PointCoord) == sizeof(double[2]));
static_assert(alignof(PointCoord) == alignof(double[2]));
struct CollectionOutput {
	IDVec data;
	IDVec indptr;
};

// except
#define THROW(CLS) throw CLS{ __FILE__, __LINE__ }

// call back intersect
struct CallBackData {
	GEOSContextHandle_t const hl;
	GEOSGeometry const* const geo;
	std::vector<GEOSGeometry const*> mapped_ptrs;
	int error_code;
};
void call_back_intersect(void* item, void* user_data);
}