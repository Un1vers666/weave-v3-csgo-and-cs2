#pragma once

#include "../sdk.hpp"

class c_planted
{
public:
    __forceinline bool bomb_ticking() {
        return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bBombTicking);
    };
    __forceinline int bomb_site() {
        return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nBombSite);
    };
    __forceinline float blow() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flC4Blow);
    };
    __forceinline float timer_length() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flTimerLength);
    };
    __forceinline float defuse_length() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flDefuseLength);
    };
    __forceinline float defuse_count_down() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flDefuseCountDown);
    };
    __forceinline float bomb_defused() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bBombDefused);
    };
    __forceinline unsigned long handle_bomb_defuser() {
          return *reinterpret_cast<unsigned long*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_hBombDefuser);
    };
};