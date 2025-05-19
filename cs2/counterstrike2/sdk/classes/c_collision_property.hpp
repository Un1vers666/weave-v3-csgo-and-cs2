#pragma once

#include "../sdk.hpp"

class c_collision_property
{
public:
    __forceinline vector mins() {
        return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_vecMins);
    };
    __forceinline vector maxs() {
        return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_vecMaxs);
    };
    __forceinline std::uint16_t solid_flags() {
        return *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_usSolidFlags);
    };
    __forceinline std::uint16_t solid_type() {
        return *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_usSolidFlags + 1);
    };

    __forceinline std::uint32_t collision_function_mask()
    {
        return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x3b);
    }
};