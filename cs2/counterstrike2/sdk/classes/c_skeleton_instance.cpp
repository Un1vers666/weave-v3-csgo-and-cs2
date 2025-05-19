// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

void c_skeleton_instance::calc_world_space_bones(uint32_t bone_mask)
{
	using function_t = void(__fastcall*)(void*, uint32_t);
	auto pattern = g_cs2->m_patterns[FNV32("calc_world_space_bones")];
	pattern.as<function_t>()(this, bone_mask);
}