// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"
#include "../../hooks/hooks.hpp"

void c_base_entity::set_model(const char* name) {

    using function_t = void* (__fastcall*)(c_base_entity*, const char*);
    g_cs2->m_patterns[FNV32("set_model")].as<function_t>()(this, name);
}

void c_base_entity::set_scene_origin(vector origin) {
    using fn = void(__fastcall*)(void*, vector);
    static auto fnorigin = g_cs2->m_patterns[FNV32("set_scene_origin")].as<fn>();
    fnorigin(this, origin);
}

void c_base_entity::update_subclass() {
    g_cs2->m_patterns[FNV32("update_subclass")].as<void(__fastcall*)(void*)>()(this);
}

std::uint32_t c_entity_instance::get_entity_handle()
{
	using function_t = std::uint32_t(__fastcall*)(void*);
	static function_t fn = g_cs2->m_patterns[FNV32("get_ent_handle")].as<function_t>(); //reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CS_XOR("48 85 C9 74 32 48 8B 41 10 48 85 C0 74 29 44")));

	//CS_ASSERT(fn != nullptr);
	return fn(this);
}