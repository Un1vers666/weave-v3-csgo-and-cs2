// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

bool c_paint_kit::uses_legacy_model()
{
	return *(bool*)(std::uintptr_t(this) + 0xB2);
}

CEconItemAttributeDefinition* c_econ_item_system::get_attribute_definition_by_name(const char* name)
{
	using function_t = CEconItemAttributeDefinition*(__fastcall*)(const char**);

	return g_cs2->m_patterns[FNV32("get_attribute_definition_by_name")].as<function_t>()(&name);
}