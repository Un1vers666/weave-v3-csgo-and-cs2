// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

//float c_base_player_weapon::get_spread()
//{
//	using function_t = float(__fastcall*)(c_base_player_weapon*);
//	return g_cs2->m_patterns[FNV32("get_spread")].as<function_t>()(this);
//}
//
//float c_base_player_weapon::get_accuracy()
//{
//	using function_t = float(__fastcall*)(c_base_player_weapon*);
//	return g_cs2->m_patterns[FNV32("get_inaccuracy")].as<function_t>()(this);
//}

float c_base_player_weapon::get_spread()
{
    return CALL_VIRTUAL(float, 357, this);
}

float c_base_player_weapon::get_accuracy() {
    float x = 0.0f, y = 0.0f;
    return CALL_VIRTUAL(float, 403, this, &x, &y);
}

void c_base_player_weapon::update_accuracy()
{
	using function_t = void(__fastcall*)(c_base_player_weapon*);
	g_cs2->m_patterns[FNV32("update_accuracy")].as<function_t>()(this);
}