// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

bool message_lite::serialize_partial_to_array(std::uintptr_t* data, int size)
{
	return g_cs2->m_patterns[FNV32("serialize_partial_to_array")].as<bool(__fastcall*)(message_lite*, std::uintptr_t*, int)>()(this, data, size);
}

void message_lite::parse_partial_to_array(const void* data, int size)
{
	return g_cs2->m_patterns[FNV32("parse_partial_to_array")].as<void(__fastcall*)(intptr_t*, const void*, int)>()(&_internal_metadata_.kUnknownFieldsTagMask, data, size);
}

std::uintptr_t* message_lite::merge(void* to, std::uintptr_t* hyeta)
{
	return g_cs2->m_patterns[FNV32("merge_to_from")].as<std::uintptr_t * (__fastcall*)(void*, intptr_t*, void*)>()(to, &_internal_metadata_.kUnknownFieldsTagMask, nullptr);
}