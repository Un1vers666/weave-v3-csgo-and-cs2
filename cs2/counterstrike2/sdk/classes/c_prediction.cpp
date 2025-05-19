// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

void c_prediction::update(int reason, int delta_tick)
{
    using function_t = void(__fastcall*)(c_prediction*, int, int);
    g_cs2->m_patterns[FNV32("prediction_update")].as<function_t>()(this, reason, delta_tick);
}