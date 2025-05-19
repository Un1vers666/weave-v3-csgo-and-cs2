#pragma once

#include "../sdk.hpp"

class c_base_model_entity;

class c_base_player_pawn : public c_base_model_entity
{
public:
    __forceinline c_player_weapon_services* weapon_services() {
        return *reinterpret_cast<c_player_weapon_services**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pWeaponServices);
    };
    __forceinline c_cs_player_item_services* item_services() {
        return *reinterpret_cast<c_cs_player_item_services**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pItemServices);
    };
    __forceinline c_player_observer_services* observer_services() {
        return *reinterpret_cast<c_player_observer_services**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pObserverServices);
    };
    __forceinline c_cs_player_camera_services* camera_services() {
        return *reinterpret_cast<c_cs_player_camera_services**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pCameraServices);
    };
    __forceinline c_player_movement_services* movement_services() {
        return *reinterpret_cast<c_player_movement_services**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pMovementServices);
    };

    __forceinline float old_sim_time() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flOldSimulationTime);
    };
};