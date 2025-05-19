// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"
#include "../../hooks/hooks.hpp"

void c_csgo_viewmodel::set_local_origin(vector origin)
{
	using function_t = void(__fastcall*)(c_csgo_viewmodel*, vector);
	g_cs2->m_patterns[FNV32("set_local_origin")].as<function_t>()(this, origin);
}