#pragma once

#include "../sdk.hpp"

class c_base_smoke_grenade_projectile : public c_base_grenade
{
public:
	__forceinline bool smoke_effect_spawned() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bSmokeEffectSpawned);
	};

	__forceinline int smoke_effect_start_tick() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + 0x1158);
	};

	__forceinline c_utl_vector<int> voxel_data() {
		return *reinterpret_cast<c_utl_vector<int>*>(reinterpret_cast<std::uint64_t>(this) + 0x1180);
	};

	__forceinline vector detonation_position() {
		return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + 0x1170);
	};
};