// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

void* i_entity_list::GetEntityByIndex(int nIndex)
{
	//@ida: #STR: "(missing),", "(missing)", "Ent %3d: %s class %s name %s\n" | or find "cl_showents" cvar -> look for callback
	//	do { pEntity = GetBaseEntityByIndex(g_pGameEntitySystem, nCurrentIndex); ... }
	add_trace(__PRETTY_FUNCTION__);
	using function_t = c_cs_player_pawn * (__fastcall*)(void*, int);
	return g_cs2->m_patterns[FNV32("get_entity_by_index")].as<function_t>()(this, nIndex);
}

c_cs_player_pawn* i_entity_list::get_local_player()
{
	add_trace(__PRETTY_FUNCTION__);
	return get_split_screen_view_player(0);
}

c_cs_player_pawn* i_entity_list::get_split_screen_view_player(int index)
{
	add_trace(__PRETTY_FUNCTION__);
	using function_t = c_cs_player_pawn * (__fastcall*)(int index);
	return g_cs2->m_patterns[FNV32("get_split_screen_view_player")].as<function_t>()(index);
}