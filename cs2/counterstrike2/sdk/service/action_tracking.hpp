#pragma once

#include "../sdk.hpp"

class c_cs_player_action_tracking_services
{
public:
    __forceinline bool m_rescuing() {
        return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bIsRescuing);
    };
};