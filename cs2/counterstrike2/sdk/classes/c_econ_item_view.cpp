// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

const char* c_econ_item_view::get_item_name(bool uncustomized)
{
	using function_t = const char* (__fastcall*)(void*, bool);
	return g_cs2->m_patterns[FNV32("localize_name")].as<function_t>()(this, uncustomized);
}