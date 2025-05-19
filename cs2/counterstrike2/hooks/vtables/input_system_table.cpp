// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../hooks.hpp"

//bool hooks::relative_mouse_mode::hook(void* input_system)
//{
//	g_cs2->m_latest_hook = LoggerHack::RelativeModeMouse;
//	bool enabled = g_gui->is_open_menu() ? false : relative_mouse_mode_original(input_system);
//	g_cs2->RelativeLastValue = enabled;
//	return enabled;
//}
void hooks::relative_mouse_mode::hook(void* input_system, bool enabled)
{
	g_cs2->m_latest_hook = LoggerHack::RelativeModeMouse;
	g_cs2->RelativeLastValue = enabled;

	if (g_gui->is_open_menu())
		return relative_mouse_mode_original(input_system, false);

	return relative_mouse_mode_original(input_system, enabled);
}