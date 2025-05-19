// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

std::uint32_t c_model::num_bones()
{
	using function_t = std::uint32_t(__fastcall*)(c_model*);
	return g_cs2->m_patterns[FNV32("num_bones")].as<function_t>()(this);
}

uint32_t c_model::hitbox_number() {

	using fnHitboxNum = uint32_t(__fastcall*)(void*);
	return g_cs2->m_patterns[FNV32("num_of_hitboxes")].as<fnHitboxNum>()(this);
}

std::uint32_t c_model::bone_flags(int index)
{
	using function_t = std::uint32_t(__fastcall*)(c_model*, int);
	return g_cs2->m_patterns[FNV32("bone_flags")].as<function_t>()(this, index);
}

std::uint32_t c_model::bone_parent(int index)
{
	using function_t = std::uint32_t(__fastcall*)(c_model*, int);
	return g_cs2->m_patterns[FNV32("bone_parent")].as<function_t>()(this, index);
}