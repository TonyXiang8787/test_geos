#pragma once

#define GEOS_USE_ONLY_R_API
#include "geos_c.h"

#include <memory>
#include <type_traits>


namespace spatial_mapper {

// global
template <auto fn>
using deleter_from_fn = std::integral_constant<
	std::decay_t<decltype(fn)>, fn>;
template <typename T, auto fn>
using unique_ptr_deleter = std::unique_ptr<T, deleter_from_fn<fn>>;
using GlobalHandleObj = std::remove_pointer_t<GEOSContextHandle_t>;
using GlobalHandle = unique_ptr_deleter<GlobalHandleObj, GEOS_finish_r>;

// handle deleter
template <class T, auto fn>
class handle_deleter {
public:
	handle_deleter(GEOSContextHandle_t handle) : hl_{ handle } {}
	void operator()(T* ptr) { fn(hl_, ptr); }
private:
	GEOSContextHandle_t const hl_;
};
template <typename T, auto fn>
using unique_ptr_handle_deleter = std::unique_ptr<T, handle_deleter<T, fn>>;
using RTreeHandle = unique_ptr_handle_deleter<
	GEOSSTRtree, GEOSSTRtree_destroy_r>;
using CoordSeqHandle = unique_ptr_handle_deleter<
	GEOSCoordSequence, GEOSCoordSeq_destroy_r>;
using GeometryHandle = unique_ptr_handle_deleter<
	GEOSGeometry, GEOSGeom_destroy_r>;
}