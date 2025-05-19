#pragma once

#include "../sdk.hpp"

class c_base_entity;

class c_base_player_controller : public c_base_entity
{
public:
    __forceinline std::uint64_t steam_id() {
        return *reinterpret_cast<std::uint64_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_steamID);
    };
    __forceinline bool is_local_player_controller() {
        return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bIsLocalPlayerController);
    };
    __forceinline int tick_base() {
        return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nTickBase);
    };
    __forceinline std::uint32_t desired_fov() {
        return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iDesiredFOV);
    };
};