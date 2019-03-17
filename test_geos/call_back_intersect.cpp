#include "geos_type_def.hpp"

namespace spatial_mapper {

void call_back_intersect(void* item, void* user_data)
{
	GEOSGeometry const* mapped_geo = reinterpret_cast<GEOSGeometry const*>(item);
	CallBackData* call_back_data = reinterpret_cast<CallBackData*>(user_data);
	char inter = GEOSIntersects_r(call_back_data->hl, call_back_data->geo, mapped_geo);
	if (inter == 1)
	{
		try {
			call_back_data->mapped_ptrs.push_back(mapped_geo);
		}
		catch (...)
		{
		}
	}
}

}