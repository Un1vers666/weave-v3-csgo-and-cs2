#pragma once

#include "../sdk.hpp"

class c_player_movement_services
{
public:
    __forceinline float m_max_speed() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flMaxspeed);
    };
    __forceinline float forward_move() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flForwardMove);
    };
    __forceinline float side_move() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flLeftMove);
    };
    __forceinline bool m_bOldJumpPressed() {
        return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bOldJumpPressed);
    };
    __forceinline float m_flDuckAmount() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flDuckAmount);
    };
    __forceinline float m_flSurfaceFriction() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flSurfaceFriction);
    };
public:
    void try_player_move(__int64 unk_target, vector position, float* unk_penis);
    void set_prediction_command(c_user_cmd* cmd);
    void reset_prediction_command();
};