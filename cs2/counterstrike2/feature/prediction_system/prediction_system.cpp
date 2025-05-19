// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../feature.hpp"

void restore_view_angle()
{
	g_cs2->m_patterns[FNV32("set_view_angle")].as<void(__fastcall*)(c_cs_player_pawn*, vector)>()(g_cs2->local_player, g_cs2->store_angle);
}

void c_prediction_system::run_prediction() {
	if (!Interfaces::m_network_game_service)
		return;

	g_cs2->m_patterns[FNV32("run_prediction")].as<void(__fastcall*)(c_network_game_client*, int)>()(Interfaces::m_network_game_service->get_network_game_client(), prediction_reason::client_command_tick);
}

void c_prediction_system::update(c_user_cmd* cmd)
{

	storage_.real_time = Interfaces::globals->realtime;
	storage_.current_time = Interfaces::globals->global_storage.curtime;
	storage_.tick_count = Interfaces::csgo_input->sequence;
	storage_.on_ground = g_cs2->local_player->flags() & fl_onground;

	if (g_cs2->local_player->get_weapon_active())
	{
		g_cs2->local_player->get_weapon_active()->update_accuracy();
		storage_.spread = g_cs2->local_player->get_weapon_active()->get_spread();
		storage_.accuracy = g_cs2->local_player->get_weapon_active()->get_accuracy();
	}
	
	storage_.command_prediction = cmd->has_been_prediction;
	storage_.game_prediction = Interfaces::m_network_game_client->get_prediction();
	storage_.prediction = Interfaces::m_prediction->in_prediction;
	storage_.first_prediction = Interfaces::m_prediction->m_first_prediction;
	
	cmd->has_been_prediction = true;
	Interfaces::m_network_game_client->set_prediction(true);
	Interfaces::m_prediction->in_prediction = true;
	Interfaces::m_prediction->m_first_prediction = false;
	
	if (Interfaces::m_network_game_client->delta_tick > 0)
		run_prediction();
	
	Interfaces::globals->global_storage.curtime = TICKS_TO_TIME(g_cs2->local_player->get_controller()->tick_base());
	Interfaces::globals->global_storage.curtime2 = TICKS_TO_TIME(g_cs2->local_player->get_controller()->tick_base());
}

void c_prediction_system::begin(c_user_cmd* cmd)
{
	if (!g_cs2->local_player)
		return;

	if (!g_cs2->local_player->is_alive())
		return;

	auto movement = g_cs2->local_player->movement_services();

	if (!movement)
		return;

	movement->set_prediction_command(cmd);
	g_cs2->local_player->get_controller()->m_current_command() = cmd;
}

void c_prediction_system::end()
{
	if (!g_cs2->local_player)
		return;

	if (!g_cs2->local_player->is_alive())
		return;

	auto movement = g_cs2->local_player->movement_services();

	if (!movement)
		return;

	movement->reset_prediction_command();

	Interfaces::globals->realtime = storage_.real_time;
	Interfaces::globals->global_storage.curtime = storage_.current_time;
	Interfaces::globals->global_storage.curtime2 = storage_.current_time;
	Interfaces::globals->global_storage.tick_count = storage_.tick_count;
	Interfaces::m_prediction->m_first_prediction = storage_.first_prediction;
	Interfaces::m_prediction->in_prediction = storage_.prediction;
	g_cs2->local_player->get_controller()->m_current_command() = nullptr;
}

void c_prediction_system::movement_processing(c_player_movement_services* movement, c_user_cmd* cmd)
{
	g_cs2->m_patterns[FNV32("movement_processing")].as<void(__fastcall*)(c_player_movement_services*, c_user_cmd*)>()(movement, cmd);
}

void c_prediction_system::set_prediction_command(c_user_cmd* command)
{
	g_cs2->m_patterns[FNV32("set_prediction_command")].as<void(__fastcall*)(c_user_cmd*)>()(command);
}

void c_prediction_system::set_prediction_data(c_player_movement_services* movement, c_user_cmd* cmd)
{
	g_cs2->m_patterns[FNV32("set_prediction_data")].as<void(__fastcall*)(c_player_movement_services*, c_user_cmd*)>()(movement, cmd);
}

void c_prediction_system::reset_prediction_data()
{
	g_cs2->m_patterns[FNV32("clear_prediction_data")].as<void(__fastcall*)()>()();
}