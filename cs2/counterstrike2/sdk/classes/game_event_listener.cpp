// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

c_cs_player_controller* c_game_event_helper::get_player_controller()
{
    if (!event)
        return nullptr;

    int controller_id{};

    c_buffer buffer;
    buffer.name = "userid";

    event->get_controller_id(controller_id, &buffer);

    if (controller_id == -1)
        return nullptr;

    return Interfaces::entity_list->get_controller(controller_id + 1);
}

c_cs_player_controller* c_game_event_helper::get_attacker_controller()
{
    if (!event)
        return nullptr;

    int controller_id{};

    c_buffer buffer;
    buffer.name = "attacker";

    event->get_controller_id(controller_id, &buffer);

    if (controller_id == -1)
        return nullptr;

    return Interfaces::entity_list->get_controller(controller_id + 1);
}

int c_game_event_helper::get_damage()
{
    if (!event)
        return -1;

    return event->get_int2("dmg_health", false);
}

int c_game_event_helper::get_health()
{
    if (!event)
        return -1;

    c_buffer buffer;
    buffer.name = "health";

    return event->get_int(&buffer);
}

int c_game_event::get_int2(const char* name, bool unk)
{
    //return CALL_VIRTUAL(int, 7, this, name, unk);
    return g_cs2->m_patterns[FNV32("get_int_event")].as<int(__thiscall*)(void*, const char*, bool)>()(this, name, unk);
}

float c_game_event::get_float2(const char* name, bool unk)
{
    return CALL_VIRTUAL(float, 9, this, name, unk);
   // return g_cs2->m_patterns[FNV32("get_float_event")].as<float(__thiscall*)(void*, const char*, bool)>()(this, name, unk);
}
