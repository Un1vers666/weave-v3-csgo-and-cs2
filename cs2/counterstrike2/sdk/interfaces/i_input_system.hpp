#pragma once

#include <cstdint>

class c_input_sys {
public:
    bool is_relative_mouse_mode();

    void* get_sdl_window() {
        // Offset in 'CInputSystem::DebugSpew'.
        // xref: "Current coordinate bias %s: %g,%g scale %g,%g\n".
        // find Plat_WindowToOsSpecificHandle first param
        return *reinterpret_cast<void**>((uintptr_t)(this) + 0x26A8);
    }
};