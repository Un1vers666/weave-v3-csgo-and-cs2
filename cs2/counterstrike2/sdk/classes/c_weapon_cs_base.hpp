#pragma once

#include "../sdk.hpp"

class c_game_scene_node;
class c_base_anim_graph_controller;
class c_base_player_weapon;

class c_weapon_cs_base : public c_base_player_weapon
{
public:
    __forceinline int recoil_index() {
        return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iRecoilIndex);
    };
    __forceinline std::uint32_t m_seq_idle() {
        return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_seqIdle);
    };
    __forceinline std::uint32_t m_seq_fire_primary() {
        return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_seqFirePrimary);
    };
    __forceinline std::uint32_t m_seq_fire_secondary() {
        return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_seqFireSecondary);
    };
    __forceinline c_attribute_container* attribute_manager() {
        return reinterpret_cast<c_attribute_container*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_AttributeManager);
    };
    __forceinline uint32_t& paint_kit() {
        return *reinterpret_cast<uint32_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nFallbackPaintKit);
    };
    __forceinline int original_team_number() {
        return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iOriginalTeamNumber);
    };
    __forceinline std::int32_t m_nPostponeFireReadyTicks() {
        return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nPostponeFireReadyTicks);
    };
    __forceinline float m_flPostponeFireReadyFrac() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flPostponeFireReadyFrac);
    };
    __forceinline bool m_bInReload() {
        return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bInReload);
    };

    vector calculate_spread(unsigned int index);
    void get_spread_pattern(unsigned short item_index, int bullets, int recoil_index, float* r3, float* r4);
    void get_spread_pattern_vfunc(unsigned short item_index, int bullets, int recoil_index, float* r3, float* r4);
public:
    auto get_ref_ehandle() {
        c_handle handle;
        CALL_VIRTUAL(void*, 2, this, &handle);
        return handle;
    }
    void calc_spread(c_weapon_cs_base_v_data* data, int mode, unsigned int seed, float innacuracy, float spread, float recoil_index, float* spread_x, float* spread_y);
};