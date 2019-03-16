#pragma once

#define GEOS_USE_ONLY_R_API
#include "geos_c.h"

#include <memory>
#include <type_traits>


namespace spatial_mapper {

template <auto fn>
using deleter_from_fn = std::integral_constant<std::decay_t<decltype(fn)>, fn>;

template <typename T, auto fn>
using unique_ptr_deleter = std::unique_ptr<T, deleter_from_fn<fn>>;


using GlobalHandleObj = std::remove_pointer_t<GEOSContextHandle_t>;
using GlobalHandle = unique_ptr_deleter<GlobalHandleObj, GEOS_finish_r>;

}