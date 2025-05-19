// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

bool i_trace::trace_shape(c_ray* ray, const vector& start, const vector& end, c_trace_filter* filter, c_game_trace* trace)
{
	using function_t = bool(__fastcall*)(i_trace*, c_ray*, const vector&, const vector&, c_trace_filter*, c_game_trace*);
	bool ret = g_cs2->m_patterns[FNV32("trace_shape")].as<function_t>()(this, ray, start, end, filter, trace);
	return ret;
}

int i_trace::point_contents(const vector& end, int mask, __int16 mask2)
{
	using function_t = int(__fastcall*)(i_trace*, const vector&, __int64, __int16);
	return g_cs2->m_patterns[FNV32("point_contents")].as<function_t>()(this, end, mask, mask2);
}

bool i_trace::clip_ray_to_entity(c_ray* ray, const vector& start, const vector& end, c_cs_player_pawn* player, c_trace_filter* filter, c_game_trace* trace)
{
	using function_t = bool(__fastcall*)(i_trace*, c_ray*, const vector&, const vector&, c_cs_player_pawn*, c_trace_filter*, c_game_trace*);
	return g_cs2->m_patterns[FNV32("clip_ray_to_entity")].as<function_t>()(this, ray, start, end, player, filter, trace);
}