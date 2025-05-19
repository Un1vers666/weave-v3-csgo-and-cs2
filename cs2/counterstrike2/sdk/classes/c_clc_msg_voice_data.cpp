// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

c_clc_msg_voice_data* c_clc_msg_voice_data::create()
{
	return g_cs2->m_patterns[FNV32("c_clc_msg_voice_data")].as<c_clc_msg_voice_data * (__fastcall*)()>()();
}