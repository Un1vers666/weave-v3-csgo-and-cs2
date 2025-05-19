#pragma once

#include "../sdk.hpp"

class c_base_cs_grenade_projectile : public c_base_grenade
{
public:
	__forceinline c_utl_vector< vector > trail_points() {
		return *reinterpret_cast<c_utl_vector< vector >*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_arrTrajectoryTrailPoints);
	};

	__forceinline c_utl_vector< float > trail_times() {
		return *reinterpret_cast<c_utl_vector< float >*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_arrTrajectoryTrailPointCreationTimes);
	};
};