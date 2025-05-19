// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../feature.hpp"
#include "../../utilities/input_system/input_system.h"

float angle_normalize(float angle)
{
	angle = fmodf(angle, 360.0f);
	if (angle > 180)
	{
		angle -= 360;
	}
	if (angle < -180)
	{
		angle += 360;
	}
	return angle;
}

void c_antiaim::run(c_user_cmd* cmd)
{
	if (!g_user->IsActive(xorstr_("antiaim_enable"), xorstr_("antiaim_enable"), 0))
		return;

	if (g_cs2->local_player->actual_move_type() == movetype_ladder || g_cs2->local_player->actual_move_type() == movetype_noclip)
		return;

	if (Interfaces::m_game_rules->freeze_pause)
		return;

	if (Interfaces::m_game_rules->game_phase == 5)
		return;

	if (cmd->buttons.hold & IN_USE)
		return;

	if (!cmd->csgo_user_cmd.base || !cmd->csgo_user_cmd.base->view)
		return;

	if (g_cs2->local_player->get_weapon_active() && g_cs2->local_player->get_weapon_active()->is_grenade())
	{
		auto grenade = (c_base_cs_grenade*)g_cs2->local_player->get_weapon_active()->get_weapon_base();
		if (grenade->throw_time() > 0)
			return;
	}
	else
	{
		if (g_cs2->did_shoot && g_ragebot->can_shoot(cmd->csgo_user_cmd.input_history.m_pRep->m_tElements[cmd->csgo_user_cmd.input_history.m_nCurrentSize - 1]->player_tick_count) && !g_user->IsActive(xorstr_("ragebot_hideshots"), xorstr_("ragebot_hideshots"), 0))
			return;
	}

	pitch(cmd);

	yaw(cmd);
}

void c_antiaim::pitch(c_user_cmd* cmd)
{
	if (!g_user->IsActive(xorstr_("antiaim_pitch"), xorstr_("antiaim_pitch"), 0))
		return;

	switch (int(g_user->IsActive(xorstr_("antiaim_pitch"), xorstr_("antiaim_pitch"), 0)))
	{
	case 1:
		cmd->csgo_user_cmd.base->view->angles.x = 89.f;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
		break;
	case 2:
		cmd->csgo_user_cmd.base->view->angles.x = -89.f;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
		break;
	}
}

float get_fov(const vector& view_angles, const vector& start, const vector& end) {
	vector dir, fw;

	// get direction and normalize.
	dir = (end - start).normalized();

	// get the forward direction vector of the view angles.
	math::angle_vectors(view_angles, fw);

	// get the angle between the view angles forward directional vector and the target location.
	return std::max(math::rad2deg(std::acos(fw.dot(dir))), 0.f);
}

void at_target(c_user_cmd* cmd) {
	if (!g_user->IsActive(xorstr_("antiaim_at_target"), xorstr_("antiaim_at_target"), 0))
		return;

	float  best_dist{ std::numeric_limits< float >::max() };
	float  dist;
	c_cs_player_pawn* target, * best_target{ nullptr };

	for (int i{ 1 }; i <= Interfaces::globals->max_client; ++i) {
		auto controller = Interfaces::entity_list->get_controller(i);

		if (!controller)
			continue;

		if (!controller->find_class(HASH("CCSPlayerController")))
			continue;

		auto pawn = controller->get_player_pawn();

		if (!pawn)
			continue;

		if (!pawn->is_enemy())
			continue;

		if (!pawn->is_alive())
			continue;

		// check if a player was closer to us.
		dist = get_fov(g_cs2->angle, g_cs2->eye_position, pawn->game_scene_node()->abs_origin());
		if (dist < best_dist) {
			best_dist = dist;
			best_target = pawn;
		}
	}

	if (best_target) {
		// todo - dex; calculate only the yaw needed for this (if we're not going to use the x component that is).
		vector angle;
		math::vector_angles(best_target->game_scene_node()->vec_origin() - g_cs2->local_player->game_scene_node()->vec_origin(), angle);
		cmd->csgo_user_cmd.base->view->angles.y = angle.y;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
	}
}

void c_antiaim::yaw(c_user_cmd* cmd)
{
	at_target(cmd);

	if (g_user->IsActive(xorstr_("antiaim_yaw"), xorstr_("antiaim_yaw"), 0) == 1)
	{
		cmd->csgo_user_cmd.base->view->angles.y += 180.f;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
	}

	cmd->csgo_user_cmd.base->view->angles.y += g_user->IsActive(xorstr_("antiaim_yaw_offset"), xorstr_("antiaim_yaw_offset"), 0);

	int jitter_type = g_user->IsActive(xorstr_("antiaim_jitter_type"), xorstr_("antiaim_jitter_type"), 0);
	float jitter_value = g_user->IsActive(xorstr_("antiaim_jitter"), xorstr_("antiaim_jitter"), 0);

	static int tick_3_way = 0;
	static bool invert{ };
	static int rotatedYaw{ };

	switch (jitter_type) {
	case JITTER_TYPE_NONE:
		break;
	case JITTER_TYPE_CENTER:
		cmd->csgo_user_cmd.base->view->angles.y += jitter_value * jitter_side;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
		break;
	case JITTER_TYPE_3_WAY:
		switch (tick_3_way)
		{
		case 0:
			cmd->csgo_user_cmd.base->view->angles.y -= jitter_value;
			cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
			break;
		case 2:
			cmd->csgo_user_cmd.base->view->angles.y += jitter_value;
			cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
			break;
		}
		++tick_3_way;
		tick_3_way %= 3;
		break;
	case JITTER_TYPE_RANDOM:
		cmd->csgo_user_cmd.base->view->angles.y += (rand() % (int)jitter_value) * jitter_side;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
		break;
	case JITTER_TYPE_SPIN:
		rotatedYaw += jitter_value;
		rotatedYaw = std::remainderf(rotatedYaw, 360.f);
		cmd->csgo_user_cmd.base->view->angles.y += rotatedYaw;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
		break;
	}

	jitter_side = -jitter_side;

	cmd->csgo_user_cmd.base->view->angles.y = angle_normalize(cmd->csgo_user_cmd.base->view->angles.y);
	cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
}