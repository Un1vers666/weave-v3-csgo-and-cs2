#pragma once

#include "../sdk.hpp"

class c_base_grenade
{
public:
	__forceinline c_handle& h_thrower() {
		return *reinterpret_cast<c_handle*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_hThrower);
	};

	__forceinline float dmg_radius() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_DmgRadius);
	};
};