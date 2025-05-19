// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "sdl.hpp"

#include <Windows.h>
#include "../../core.hpp"

void sdl::set_relative_mouse_mode(bool mode)
{
	using function_t = std::uint64_t(__stdcall*)(bool);
	function_t fn = g_cs2->m_patterns[FNV32("set_relative_mouse_mode")].as<function_t>();

	if (!fn)
		return;

	fn(mode);
}

void sdl::get_relative_mouse_mode(void* mode)
{
	using function_t = std::uint64_t(__stdcall*)(void);
	function_t fn = g_cs2->m_patterns[FNV32("get_relative_mouse_mode")].as<function_t>();

	if (!fn)
		return;

	fn();
}

void sdl::set_window_polling_mode(void* window, bool mode)
{
	using function_t = void(__stdcall*)(void*, bool);
	function_t fn = g_cs2->m_patterns[FNV32("set_window_grab")].as<function_t>();

	if (!fn)
		return;

	fn(window, mode);
}

void sdl::set_mouse_warp_position(void* window, std::int32_t x, std::int32_t y)
{
	using function_t = void(__stdcall*)(void*, std::int32_t, std::int32_t);
	function_t fn = g_cs2->m_patterns[FNV32("warp_mouse_in_window")].as<function_t>();

	if (!fn)
		return;

	fn(window, x, y);
}
