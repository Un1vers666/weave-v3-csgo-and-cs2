// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

c_msg_voice_audio* c_msg_voice_audio::create()
{
	return g_cs2->m_patterns[FNV32("c_msg_voice_audio")].as<c_msg_voice_audio * (__fastcall*)(void*)>()(nullptr);
}