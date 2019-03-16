#pragma once

#include "geos_helper.hpp"


namespace spatial_mapper {

class Mapper {
public:
	Mapper() :
		global_handle_{ GEOS_init_r() }
	{}
	GEOSContextHandle_t global_handle() { return global_handle_.get(); }
private:
	GlobalHandle const global_handle_;
};

}