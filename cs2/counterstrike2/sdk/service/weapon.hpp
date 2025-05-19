#pragma once

#include <cstdint>
#include "../math/vector.hpp"
#include "../sdk.hpp"

class c_player_weapon_services
{
public:
	__forceinline c_handle m_hactive_weapon() {
		return *reinterpret_cast<c_handle*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_hActiveWeapon);
	};
	__forceinline c_utl_vector<c_handle> m_hmyweapons() {
		return *reinterpret_cast<c_utl_vector<c_handle>*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_hMyWeapons);
	};
	__forceinline float next_attack() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flNextAttack);
	};
public:
	__forceinline vector get_shoot_pos()
	{
		vector temp;
		CALL_VIRTUAL(void, 24, this, &temp); //23
		return temp;
	}
	//__forceinline vector get_shoot_pos() {
	//	return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + 0x184);
	//};
	bool weapon_owns_this_type(const char* name);

	c_shoot_position_history_data* get_shoot_position_history() const {
		auto shoot_position_data = reinterpret_cast<c_shoot_position_history_data*>(uintptr_t(this) + 0xC0);
		return shoot_position_data;
	}
};