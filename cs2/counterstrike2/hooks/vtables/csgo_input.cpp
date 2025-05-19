// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../hooks.hpp"
#include "../../Logging.h"
#include <tinyformat/tinyformat.h>
#include "../../sdk/classes/c_grenade_trace.h"
#include "../sdk/crc.h"

void bombandir(void* ptr)
{
	//remove entity
	g_cs2->m_patterns[FNV32("delete_entity")].as<void(__fastcall*)(void* ptr)>()(ptr);
}

bool valid_throw(c_cs_player_pawn* pawn, float value)
{
	return g_cs2->m_patterns[FNV32("valid_throw")].as<bool(__fastcall*)(void* ptr, float)>()(pawn, value);
}

void get_time(float& time, int group)
{
	g_cs2->m_patterns[FNV32("get_time")].as<void(__fastcall*)(float&, int)>()(time, group);
}

std::uintptr_t* get_binding()
{
	return g_cs2->m_patterns[FNV32("binding")].as<std::uintptr_t*>();
}

void shared()
{
	auto voice_data_ = Interfaces::m_network_messages->find_or_create_net_message(22, get_binding());

	auto voice_message = c_clc_msg_voice_data::create();
	voice_message->audio = c_msg_voice_audio::create();

	voice_message->m_has_bits = 7;
	voice_message->tick = 1;
	voice_message->audio->has_bits = 0x1F;

	voice_message->audio->format = 0;
	voice_message->xuid = FNV64("weave");
	voice_message->audio->sample_rate = 35000;
	voice_message->audio->section_number = 1;

	Interfaces::m_network_game_service->send_voice(voice_data_, voice_message);

	Interfaces::m_mem_alloc->Free(voice_message->audio);
	Interfaces::m_mem_alloc->Free(voice_message);
}

void correct_movement(c_user_cmd* cmd, vector_2d& wish_move) {

	if (!cmd->csgo_user_cmd.base || !cmd->csgo_user_cmd.base->view)
		return;

	float wish_y, view_y{};

	if (g_cs2->store_angle.y < 0.f)
		wish_y = 360.f + g_cs2->store_angle.y;
	else
		wish_y = g_cs2->store_angle.y;

	if (cmd->csgo_user_cmd.base->view->angles.y < 0.f)
		view_y = 360.f + cmd->csgo_user_cmd.base->view->angles.y;
	else
		view_y = cmd->csgo_user_cmd.base->view->angles.y;

	float delta_view{};

	if (view_y < wish_y)
		delta_view = std::abs(view_y - wish_y);
	else
		delta_view = 360.f - std::abs(wish_y - view_y);

	delta_view = 360.f - delta_view;

	cmd->csgo_user_cmd.base->forwardmove = std::cos(math::deg2rad(delta_view)) * wish_move.x
		+ -std::cos(math::deg2rad(delta_view + 90.f)) * wish_move.y;
	cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_FORWARDMOVE);

	cmd->csgo_user_cmd.base->sidemove = -std::sin(math::deg2rad(delta_view)) * wish_move.x
		+ std::sin(math::deg2rad(delta_view + 90.f)) * wish_move.y;
	cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);

	if (g_cs2->store_angle.y < 0.0)
		g_cs2->store_angle.y += 360.0;
	
	if (cmd->csgo_user_cmd.base->view->angles.y < 0.0)
	{
		cmd->csgo_user_cmd.base->view->angles.y += 360.0;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
	}
}

int get_player_tick(std::uintptr_t* container) {
	return *g_cs2->m_patterns[FNV32("get_player_tick")].as<int* (__fastcall*)(std::uintptr_t*, std::uintptr_t*, char)>()(nullptr, container, 0);
}

unsigned int get_hash_seed2(c_cs_player_pawn* player, vector angle, int attack_tick)
{
	return g_cs2->m_patterns[FNV32("get_hash_seed")].as<unsigned int(__fastcall*)(c_cs_player_pawn*, vector, int)>()(player, angle, attack_tick);
}

void compensation_spread(c_user_cmd* cmd) {
	auto weapon = g_cs2->local_player->get_weapon_active();

	if (!weapon)
		return;

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	auto econ_item_view = weapon->get_econ_view_item();

	if (!econ_item_view)
		return;

	static bool penis = false;

	if (!(cmd->csgo_user_cmd.base->buttons->hold & 1)) {
		penis = false;
		return;
	}

	if (penis) {
		return;
	}

	weapon = g_cs2->local_player->get_weapon_active();

	weapon->update_accuracy();

	vector dir{}, end{}, fwd{}, right{}, up{};

	c_game_trace trace{};

	c_trace_filter filter(0x1C3003, g_cs2->local_player, nullptr, 4);

	int hits{};
	float range = weapon_data->range();

	auto angle_ = cmd->csgo_user_cmd.input_history.m_pRep->m_tElements[cmd->csgo_user_cmd.input_history.m_nCurrentSize - 1]->angle->angles;

	math::angle_vectors(angle_, &fwd, &right, &up);

	auto spread_calc = weapon->calculate_spread(get_hash_seed2(g_cs2->local_player, angle_, cmd->csgo_user_cmd.input_history.m_pRep->m_tElements[cmd->csgo_user_cmd.input_history.m_nCurrentSize - 1]->player_tick_count));

	// get spread direction.
	dir = vector(fwd + (right * spread_calc.x) + (up * spread_calc.y));

	vector p{};

	math::vector_angles(dir, p);
	math::normalize_angles(p);

	vector pizda = angle_ - p;

	vector pizda2 = vector(angle_.x + pizda.x, angle_.y - pizda.y, angle_.z);
	vector pizda3 = vector(Interfaces::csgo_input->GetViewAngles().x - pizda.x, Interfaces::csgo_input->GetViewAngles().y + pizda.y, Interfaces::csgo_input->GetViewAngles().z);

	cmd->set_input_history_angle(pizda3);

	auto final_angle = vector(Interfaces::csgo_input->GetViewAngles().x - pizda.x, Interfaces::csgo_input->GetViewAngles().y + pizda.y, Interfaces::csgo_input->GetViewAngles().z);
	Interfaces::csgo_input->SetViewAngle(final_angle);
	penis = true;
}

void __fastcall hooks::create_move::hook(i_csgo_input* input, unsigned int slot, char a3)
{
	g_cs2->m_latest_hook = LoggerHack::CreateMove;

	create_move_original(input, slot, a3);

	g_cs2->local_player = Interfaces::entity_list->get_split_screen_view_player(0);

	auto cmd = input->get_user_cmd(input->sequence);
	if (cmd == nullptr)
		return create_move_original(input, slot, a3);

	c_user_cmd_base* base = cmd->csgo_user_cmd.base;
	if (base == nullptr)
		return create_move_original(input, slot, a3);

	if (!g_cs2->local_player || !g_cs2->local_player->is_alive())
	{
		g_cs2->m_lag_skeletons.clear();
		return;
	}

	if (!g_cs2->local_player->weapon_services())
		return;

	base->subtick_move.m_nCurrentSize = 0;

	std::uintptr_t v200;

	auto player_tick = get_player_tick(&v200) - 1;

	//shared();
	g_cs2->store_angle = math::normalize(base->view->angles);
	g_cs2->angle = Interfaces::csgo_input->GetViewAngles();
	g_cs2->should_register = true;
	g_cs2->prediction = Interfaces::m_network_game_service->get_network_game_client()->get_prediction();
	g_cs2->eye_position = g_cs2->local_player->weapon_services()->get_shoot_pos();
	g_cs2->prediction_tick = player_tick;
	g_cs2->m_current_group = g_cs2->get_weapon_group();
	g_cs2->store_position[input->sequence % 150] = g_cs2->eye_position + (g_cs2->local_player->velocity() * 0.03f);
	g_cs2->m_current_command = cmd;

	g_movement->on_pre(cmd);
	g_antiaim->run(cmd);
	
	g_prediction_system->update(cmd);
	g_prediction_system->begin(cmd);
	
	g_cs2->set_flags_weave();
	g_movement->on_post();
	g_misc->on_post();
	g_ragebot->on_post(cmd);
	g_legit_bot->on_post(cmd);
	g_cs2->auto_pistols();
	g_cs2->rapid_fire(cmd, base);

	vector_2d wish_move{ base->forwardmove, base->sidemove };

	g_prediction_system->end();

	correct_movement(cmd, wish_move);

	crc::save(base);
	if (crc::calculate_crc(base) == true);
	crc::apply(cmd);
}

void __fastcall hooks::verify_input::hook(i_csgo_input* input, int a2)
{
	g_cs2->m_latest_hook = LoggerHack::VerifyInput;
	auto angle = Interfaces::csgo_input->GetViewAngles();
	Interfaces::csgo_input->SetViewAngle(g_cs2->store_angle);
	verify_input_original(input, a2);
	Interfaces::csgo_input->SetViewAngle(angle);
}