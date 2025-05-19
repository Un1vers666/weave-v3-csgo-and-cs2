#pragma once

#include "../sdk.hpp"

class c_base_player_pawn;

class c_cs_player_pawn_base : public c_base_player_pawn
{
public:
	__forceinline c_cs_player_action_tracking_services action_tracking_services() {
		return *reinterpret_cast<c_cs_player_action_tracking_services*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pActionTrackingServices);
	};
	__forceinline c_cs_player_ping_services ping_services() {
		return *reinterpret_cast<c_cs_player_ping_services*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pPingServices);
	};
	__forceinline c_cs_player_viewmodel_services* viewmodel_services() {
		return *reinterpret_cast<c_cs_player_viewmodel_services**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pViewModelServices);
	};
	__forceinline bool is_scoped() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bIsScoped);
	};
	__forceinline bool is_walking() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bIsWalking);
	};
	__forceinline int armor_value() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_ArmorValue);
	};
	__forceinline bool has_gun_immunity() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bGunGameImmunity);
	};
	__forceinline vector get_aim_punch() {
		return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_aimPunchAngle);
	};
	__forceinline c_handle handle_controller() {
		return *reinterpret_cast<c_handle*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_hOriginalController);
	};
	__forceinline float m_flLastSmokeOverlayAlpha() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flLastSmokeOverlayAlpha);
	};
	__forceinline int shots_fired() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iShotsFired);
	};
	__forceinline bool m_bWaitForNoAttack() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bWaitForNoAttack);
	}
};