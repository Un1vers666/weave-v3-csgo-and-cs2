#pragma once

#include "../sdk.hpp"
#include "../interfaces/virtual.hpp"

class c_particle_manager
{
public:
	void create_snapshot(strong_handle<c_particle_snapshot>* out_particle_snapshot)
	{
		__int64 nUnknown = 0;
		CALL_VIRTUAL(void, 42, this, out_particle_snapshot, &nUnknown);
	}
	void draw(strong_handle<c_particle_snapshot>* particle_snapshot, int count, void* data)
	{
		CALL_VIRTUAL(void, 43, this, particle_snapshot, count, data);
	}
};