// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "c_game_particle_manager_system.hpp"
#include "../../hooks/hooks.hpp"

void c_game_particle_manager_system::create_effect_index(unsigned int* effect_index, particle_effect* effect_data)
{
	g_cs2->m_patterns[FNV32("create_effect_index")].as<void(__fastcall*)(c_game_particle_manager_system*, unsigned int*, particle_effect*)>()(this, effect_index, effect_data);
}

void c_game_particle_manager_system::create_effect(unsigned int effect_index, int unk, void* clr, int unk2)
{
	g_cs2->m_patterns[FNV32("create_effect")].as<void(__fastcall*)(c_game_particle_manager_system*, unsigned int, int, void*, int)>()(this, effect_index, unk, clr, unk2);
}

void c_game_particle_manager_system::unk_function(int effect_index, unsigned int unk, const strong_handle<c_particle_snapshot>* particle_snapshot)
{
	g_cs2->m_patterns[FNV32("unk_function")].as<bool(__fastcall*)(c_game_particle_manager_system*, int, unsigned int, const strong_handle<c_particle_snapshot>*)>()(this, effect_index, unk, particle_snapshot);
}