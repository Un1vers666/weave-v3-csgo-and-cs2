#pragma once

#include "../sdk.hpp"

class c_cs_player_item_services
{
public:
    __forceinline bool m_has_defuser() {
        return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bHasDefuser);
    };
    __forceinline bool m_has_helmet() {
        return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bHasHelmet);
    };
    __forceinline bool m_has_heavy_armor() {
        return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bHasHeavyArmor);
    };
};