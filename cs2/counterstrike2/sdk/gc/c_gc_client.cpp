// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk/sdk.hpp"

c_gc_client_shared_object_cache* c_gc_client::find_so_cache(soid_t ID, bool bCreateIfMissing)
{
    using function_t = c_gc_client_shared_object_cache * (__fastcall*)(c_gc_client*, soid_t, bool);
    return g_cs2->m_patterns[FNV32("find_so_cache")].as<function_t>()(this, ID, bCreateIfMissing);
}