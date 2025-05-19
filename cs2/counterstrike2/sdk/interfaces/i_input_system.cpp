#include "../sdk.hpp"

bool c_input_sys::is_relative_mouse_mode()
{
    using function_t = bool(__stdcall*)(void);
    function_t fn = g_cs2->m_patterns[FNV32("get_relative_mouse_mode")].as<function_t>();

    return fn();
}
