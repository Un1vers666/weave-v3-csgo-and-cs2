// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../hooks.hpp"
#include "../../utilities/hotkey_system/hotkey_system.h"

class view2
{
public:
	char pad[0x4F8];
	vector angle;
};

void hooks::level_shutdown::hook(std::uintptr_t* unk)
{
	g_cs2->m_latest_hook = LoggerHack::LevelShutdown;
	add_trace(__PRETTY_FUNCTION__);
	g_cs2->local_player = nullptr;
	g_cs2->killed_target = nullptr;
	g_visuals->timer = -1;
	g_visuals->damage = -1;
	Interfaces::m_render_game_system = nullptr;
	g_visuals->m_players.clear();
	g_ragebot->m_better_player = std::nullopt;
	g_cs2->m_lag_skeletons.clear();

	return level_shutdown_original(unk);
}

void __fastcall  hooks::override_view::hook(c_client_mode* a1, __int64 a2, __int64 a3, __int64 a4, unsigned long a5)
{
	g_cs2->m_latest_hook = LoggerHack::OverrideView;
	add_trace(__PRETTY_FUNCTION__);

	g_cs2->local_player = Interfaces::entity_list->get_split_screen_view_player(0);

	if (!Interfaces::engine->is_connected() || !Interfaces::engine->is_in_game())
		return override_view_original(a1, a2, a3, a4, a5);

	if (!g_cs2->local_player)
		return override_view_original(a1, a2, a3, a4, a5);

	Interfaces::csgo_input->can_thirdperson = g_user->IsActive(xorstr_("misc_thirdperson"), xorstr_("misc_thirdperson"),0);


	c_trace_filter filter(0x3001, nullptr, nullptr, 3);
	filter.collision2 = 0x10000;

	if (Interfaces::csgo_input->can_thirdperson && g_cs2->local_player->is_alive())
	{
		auto angle = Interfaces::csgo_input->GetViewAngles();

		vector fwe;

		math::angle_vectors(angle, &fwe, nullptr, nullptr);

		angle.z = g_user->IsActive(xorstr_("misc_thirdperson_amount"), xorstr_("misc_thirdperson_amount"),0);

		c_ray ray;
		c_game_trace trace;
		Interfaces::trace->trace_shape(&ray, g_cs2->eye_position, g_cs2->eye_position - (fwe * angle.z), &filter, &trace);

		if (trace.fraction < 1.f)
			angle.z *= trace.fraction;

		Interfaces::csgo_input->can_thirdperson = trace.fraction > 0.15;
		Interfaces::csgo_input->camera_angle = angle;
	}
	else {
		Interfaces::csgo_input->can_thirdperson = false;
	}

	return override_view_original(a1, a2, a3, a4, a5);
}

void __fastcall hooks::level_init::hook(void* client_mode, const char* new_map)
{
	g_cs2->m_latest_hook = LoggerHack::LevelInit;
	add_trace(__PRETTY_FUNCTION__);
	Interfaces::m_network_game_client = Interfaces::m_network_game_service->get_network_game_client();
	Interfaces::globals = g_cs2->m_patterns[FNV32("global_vars")].cast<c_global_vars*>(); //g_utils->pattern_scan(g_cs2->m_module_system.get_client(), "48 89 0D ? ? ? ? 48 89 41").get_absolute_address(3, 0).cast<c_global_vars*>();
	Interfaces::m_render_game_system = g_cs2->m_patterns[FNV32("render_game_system")].cast<c_render_game_system*>();
	g_cs2->map = new_map;
	Interfaces::m_game_rules = g_cs2->m_patterns[FNV32("game_rules")].cast<c_cs_game_rules*>();
	Interfaces::m_scene_view_debug_overlays = g_cs2->m_patterns[FNV32("get_scene_view_debug_overlay")].cast<c_scene_view_debug_overlays*>();
	Interfaces::m_particle_manager = g_cs2->m_patterns[FNV32("particle_manager")].cast<c_particle_manager*>();
	Interfaces::m_game_particle_manager_system = g_cs2->m_patterns[FNV32("game_particle_manager")].as<c_game_particle_manager_system * (__fastcall*)()>()();;
	g_visuals->m_players.clear();

	Interfaces::pvs_manager->set_pvs(false);

	return level_init_original(client_mode, new_map);
}