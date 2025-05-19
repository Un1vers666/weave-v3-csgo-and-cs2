// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../feature.hpp"
#include "../../utilities/hotkey_system/hotkey_system.h"
#include "../../utilities/input_system/input_system.h"

void c_movement::on_pre(c_user_cmd* cmd) {
	g_movement->limit_speed(cmd);
	g_movement->auto_strafe(cmd);
	g_movement->bhop(cmd);
	g_movement->fast_stop(cmd);
}

void c_movement::on_post() {
	g_movement->autopeek();
	//g_movement->duck_assist();
}

void c_movement::clamp_movement(c_user_cmd* cmd, float speed)
{
	float final_speed = speed;

	if (!g_cs2->local_player && !g_cs2->local_player->is_alive())
		return;

	if (!cmd)
		return;

	cmd->set_button_state(InputBitMask_t::IN_SPEED, EInButtonState::IN_BUTTON_DOWN_UP);

	auto f = cmd->csgo_user_cmd.base->forwardmove * 250.f;
	auto f2 = cmd->csgo_user_cmd.base->sidemove * 250.f;

	float squirt = std::sqrtf((f * f) + (f2 * f2));

	if (squirt > speed) {

		float aye = squirt / speed;
		cmd->csgo_user_cmd.base->forwardmove /= aye;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_FORWARDMOVE);
		cmd->csgo_user_cmd.base->sidemove /= aye;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);
	}
}

void c_movement::bhop(c_user_cmd* cmd)
{
	if (!g_cs2->local_player || !g_cs2->local_player->is_alive())
		return;

	if (!Interfaces::engine->is_connected() || !Interfaces::engine->is_in_game()) return;
	if (!g_cs2->local_player || !g_cs2->local_player->is_alive()) return;
	if (g_cs2->local_player->move_type() == movetype_t::movetype_ladder || g_cs2->local_player->move_type() == movetype_t::movetype_noclip) return;
	if (g_cs2->sv_autobunnyhopping && g_cs2->sv_autobunnyhopping->GetValue<bool>()) return;
	if (Interfaces::m_game_rules->freeze_pause) return;
	if (!g_user->IsActive(xorstr_("misc_bunnyhop"), xorstr_("misc_bunnyhop"), 0)) return;

	if (!should_fake)

	{
		if (cmd->csgo_user_cmd.base->buttons->hold & IN_JUMP)
		{
			is_subtick = Interfaces::csgo_input->subtick_count >= 1 || Interfaces::csgo_input->subtick_cache.size >= 1;
			should_fake = true;
		}
	}
	else if (!(cmd->csgo_user_cmd.base->buttons->hold & IN_JUMP))
	{
		should_fake = false;
	}

	m_jump[Interfaces::csgo_input->sequence % 150].on_ground = g_cs2->local_player->flags() & fl_onground;
	m_jump[Interfaces::csgo_input->sequence % 150].should_check = false;

	if (!m_jump[(Interfaces::csgo_input->sequence - 1) % 150].should_check)
	{
		bool jump_subtick = !m_jump[(Interfaces::csgo_input->sequence - 1) % 150].on_ground && m_jump[Interfaces::csgo_input->sequence % 150].on_ground && Interfaces::globals->interval_per_ticks < 0.013f;
		if (!m_jump[Interfaces::csgo_input->sequence % 150].on_ground || jump_subtick)
		{
			cmd->buttons.hold &= ~IN_JUMP;
			cmd->buttons.toggle &= ~IN_JUMP;
			cmd->buttons.scroll &= ~IN_JUMP;

			if (jump_subtick)
				m_jump[Interfaces::csgo_input->sequence % 150].should_check = true;

			return;
		}
	}

}

void c_movement::autopeek()
{
	if (!g_cs2->local_player || !g_cs2->local_player->is_alive())
		return;

	auto m_data = &m_autopeek;
	if (g_user->IsActive(xorstr_("misc_autopeek"), xorstr_("misc_autopeek"), 0)) {
		if (!m_data->m_autopeeking) {
			m_data->m_should_retrack = false;
			m_data->m_old_origin = g_cs2->local_player->game_scene_node()->abs_origin();


			c_game_trace trace;
			c_trace_filter filter(0x3001, nullptr, nullptr, 3);
			filter.collision2 = 0x10000;

			c_ray ray{};
			Interfaces::trace->trace_shape(&ray, g_cs2->eye_position, g_cs2->eye_position - vector(0, 0, 1000.f), &filter, &trace);

			if (trace.fraction < 1.0f)
				m_data->m_old_origin = trace.end_pos + vector(0.0f, 0.0f, 2.0f);

		}
		if (m_data->m_should_retrack) {
			auto distance = g_cs2->local_player->game_scene_node()->abs_origin().dist_to(m_data->m_old_origin);
			if (distance <= 16.0) {
				m_data->m_autopeeking = false;
				m_data->m_should_retrack = false;
			}
			else {
				vector ang_to_pos;
				vector local_angle = Interfaces::csgo_input->GetViewAngles();
				auto difference = m_data->m_old_origin - g_cs2->local_player->game_scene_node()->abs_origin();
				math::vector_angles(difference, ang_to_pos);
				auto v45 = cos(ang_to_pos.x * 0.017453292);

				g_cs2->m_current_command->csgo_user_cmd.base->forwardmove = 0.0f;
				g_cs2->m_current_command->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_FORWARDMOVE);
				g_cs2->m_current_command->csgo_user_cmd.base->sidemove = 0.0f;
				g_cs2->m_current_command->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);

				auto v48 = math::deg2rad((local_angle.y - ang_to_pos.y));
				auto v66 = math::deg2rad((local_angle.y - ang_to_pos.y));
				auto cos_v48 = cos(v48);
				auto sin_v48 = sin(v66);

				auto v50 = cos_v48 * v45;
				g_cs2->m_current_command->csgo_user_cmd.base->forwardmove = fmaxf(fminf(1.0f, v50), -1.0f);
				g_cs2->m_current_command->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_FORWARDMOVE);
				g_cs2->m_current_command->csgo_user_cmd.base->sidemove = -fmaxf(fminf(1.0f, (float)(sin_v48 * v45)), -1.0f);
				g_cs2->m_current_command->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);
			}
		}
		if (g_cs2->did_shoot) {
			m_data->m_should_retrack = true;
		}
		m_data->m_autopeeking = true;
	}
	else {
		m_data->m_autopeeking = false;
		m_data->m_should_retrack = false;
	}

}

void c_movement::duck_assist()
{
	if (!g_cs2->local_player->get_weapon_active())
		return;

	if (!g_user->IsActive(xorstr_("misc_duck_assist"), xorstr_("misc_duck_assist"), 0))
	{
		m_duck_assist.m_peeking = false;
		return;
	}

	if (g_cs2->local_player->get_weapon_active()->is_knife())
	{
		m_duck_assist.m_peeking = false;
		return;
	}

	if (g_cs2->local_player->get_weapon_active()->is_grenade())
	{
		m_duck_assist.m_peeking = false;
		return;
	}

	if (m_duck_assist.m_peeking)
	{
		g_cs2->m_current_command->buttons.hold &= ~InputBitMask_t::IN_DUCK;
	}

	vector fwd;
	math::angle_vectors(Interfaces::csgo_input->GetViewAngles(), fwd);

	auto shoot_position_unduck = g_cs2->eye_position + Interfaces::m_game_rules->get_view_vectors()->vec_view;

	auto point = shoot_position_unduck + (fwd * g_cs2->local_player->get_weapon_active()->get_weapon_data()->range());
	auto direction = (point - shoot_position_unduck).normalized();
	direction *= g_cs2->local_player->get_weapon_active()->get_weapon_data()->range();
	float damage = 0;

	for (int i = 1; i < 32; i++)
	{
		auto controller = Interfaces::entity_list->get_controller(i);

		if (!controller)
			continue;

		auto pawn = controller->get_player_pawn();

		if (!pawn)
			continue;

		if (pawn == g_cs2->local_player)
			continue;

		if (!pawn->is_enemy())
			continue;

		if (!pawn->is_alive())
			continue;

		int hitbox_ = -1;

		if (g_fx_firebullet->run(pawn, shoot_position_unduck, direction, damage, hitbox_))
		{
			if (g_cs2->local_player->movement_services()->m_flDuckAmount() > 0.95f)
				m_duck_assist.m_peeking = true;
		}
	}
}

void c_movement::auto_strafe(c_user_cmd* cmd)
{
	if (!g_cs2->local_player || !g_cs2->local_player->is_alive())
		return;

	if (g_cs2->local_player->move_type() == movetype_ladder || g_cs2->local_player->move_type() == movetype_noclip || g_cs2->local_player->flags() & fl_onground)
		return;

	if (g_cs2->local_player->actual_move_type() == movetype_ladder || g_cs2->local_player->actual_move_type() == movetype_noclip)
		return;

	if (g_input_system->get_hold_key(VK_LSHIFT) || g_user->IsActive(xorstr_("ragebot_slowwalk"), xorstr_("ragebot_slowwalk"), 0))
		return;

	if (!g_user->IsActive(xorstr_("misc_autostrafe"), xorstr_("misc_autostrafe"), 0))
		return;

	const auto current_buttons = cmd->buttons.hold;
	auto yaw = math::normalize_yaw(cmd->csgo_user_cmd.base->view->angles.y);

	const auto check_button = [&](const uint64_t button)
		{
			if (current_buttons & button && (!(last_buttons & button) || button & InputBitMask_t::IN_MOVELEFT && !(last_pressed & InputBitMask_t::IN_MOVERIGHT) || button &
				InputBitMask_t::IN_MOVERIGHT && !(last_pressed & InputBitMask_t::IN_MOVELEFT) || button & InputBitMask_t::IN_FORWARD && !(last_pressed & InputBitMask_t::IN_BACK) ||
				button & InputBitMask_t::IN_BACK && !(last_pressed & InputBitMask_t::IN_FORWARD)))
			{
				if (button & InputBitMask_t::IN_MOVELEFT)
					last_pressed &= ~InputBitMask_t::IN_MOVERIGHT;
				else if (button & InputBitMask_t::IN_MOVERIGHT)
					last_pressed &= ~InputBitMask_t::IN_MOVELEFT;
				else if (button & InputBitMask_t::IN_FORWARD)
					last_pressed &= ~InputBitMask_t::IN_BACK;
				else if (button & InputBitMask_t::IN_BACK)
					last_pressed &= ~InputBitMask_t::IN_FORWARD;

				last_pressed |= button;

			}
			else if (!(current_buttons & button))
				last_pressed &= ~button;
		};

	check_button(InputBitMask_t::IN_MOVELEFT);
	check_button(InputBitMask_t::IN_MOVERIGHT);
	check_button(InputBitMask_t::IN_FORWARD);
	check_button(InputBitMask_t::IN_BACK);

	last_buttons = current_buttons;

	const auto velocity = g_cs2->local_player->abs_velocity();

	if (g_cs2->local_player->flags() & fl_onground)
		return;

	auto offset = 0.f;
	if (last_pressed & InputBitMask_t::IN_MOVELEFT)
		offset += 90.f;
	if (last_pressed & InputBitMask_t::IN_MOVERIGHT)
		offset -= 90.f;
	if (last_pressed & InputBitMask_t::IN_FORWARD)
		offset *= 0.5f;
	else if (last_pressed & InputBitMask_t::IN_BACK)
		offset = -offset * 0.5f + 180.f;

	yaw += offset;

	auto velocity_angle = math::rad2deg(std::atan2f(velocity.y, velocity.x));
	if (velocity_angle < 0.0f)
		velocity_angle += 360.0f;

	if (velocity_angle < 0.0f)
		velocity_angle += 360.0f;

	velocity_angle -= floorf(velocity_angle / 360.0f + 0.5f) * 360.0f;

	const auto speed = velocity.length_2d();
	auto ideal = 0.f;

	if (speed > 0.f)
		ideal = std::clamp(math::rad2deg(std::atan2(15.f, speed)), 0.0f, 90.0f);

	auto correct = ideal;

	cmd->csgo_user_cmd.base->forwardmove = 0.f;
	cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_FORWARDMOVE);
	const auto velocity_delta = math::normalize_yaw(yaw - velocity_angle);

	// get our viewangle change.
	auto delta = math::normalize_yaw(yaw - m_old_yaw);

	// convert to absolute change.
	auto abs_delta = std::abs(delta);

	// save old yaw for next call.
	m_old_yaw = yaw;

	auto rotate_movement = [](c_user_cmd* cmd, float target_yaw)
		{
			const float rot = math::deg2rad(cmd->csgo_user_cmd.base->view->angles.y - target_yaw);

			const float new_forward = std::cos(rot) * cmd->csgo_user_cmd.base->forwardmove - std::sin(rot) * cmd->csgo_user_cmd.base->sidemove;
			const float new_side = std::sin(rot) * cmd->csgo_user_cmd.base->forwardmove + std::cos(rot) * cmd->csgo_user_cmd.base->sidemove;

			cmd->buttons.hold &= ~(InputBitMask_t::IN_BACK | InputBitMask_t::IN_FORWARD | InputBitMask_t::IN_MOVELEFT | InputBitMask_t::IN_MOVERIGHT);
			cmd->buttons.toggle &= ~(InputBitMask_t::IN_BACK | InputBitMask_t::IN_FORWARD | InputBitMask_t::IN_MOVELEFT | InputBitMask_t::IN_MOVERIGHT);

			cmd->csgo_user_cmd.base->forwardmove = std::clamp(new_forward, -1.f, 1.f);
			cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_FORWARDMOVE);
			cmd->csgo_user_cmd.base->sidemove = std::clamp(new_side * -1.f, -1.f, 1.f);
			cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);

			if (cmd->csgo_user_cmd.base->forwardmove > 0.0f)
			{
				//Interfaces::csgo_input->add_button(InputBitMask_t::IN_FORWARD);
				cmd->csgo_user_cmd.base->buttons->hold |= InputBitMask_t::IN_FORWARD;
				cmd->csgo_user_cmd.base->buttons->set_bits(InputBitMask_t::IN_FORWARD);
			}
			else if (cmd->csgo_user_cmd.base->forwardmove < 0.0f)
			{
				//Interfaces::csgo_input->add_button(InputBitMask_t::IN_BACK);
				cmd->csgo_user_cmd.base->buttons->hold |= InputBitMask_t::IN_BACK;
				cmd->csgo_user_cmd.base->buttons->set_bits(InputBitMask_t::IN_BACK);
			}

			if (cmd->csgo_user_cmd.base->sidemove > 0.0f)
			{
				//Interfaces::csgo_input->add_button(InputBitMask_t::IN_MOVELEFT);
				cmd->csgo_user_cmd.base->buttons->hold |= InputBitMask_t::IN_MOVELEFT;
				cmd->csgo_user_cmd.base->buttons->set_bits(InputBitMask_t::IN_MOVELEFT);
			}
			else if (cmd->csgo_user_cmd.base->sidemove < 0.0f)
			{
				//Interfaces::csgo_input->add_button(InputBitMask_t::IN_MOVERIGHT);
				cmd->csgo_user_cmd.base->buttons->hold |= InputBitMask_t::IN_MOVERIGHT;
				cmd->csgo_user_cmd.base->buttons->set_bits(InputBitMask_t::IN_MOVERIGHT);
			}
		};

	if (fabsf(velocity_delta) > 170.0f && speed > 80.0f || velocity_delta > correct && speed > 80.0f)
	{
		yaw = correct + velocity_angle;
		cmd->csgo_user_cmd.base->sidemove = -1.0f;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);
		rotate_movement(g_cs2->m_current_command, math::normalize_yaw(yaw));
		return;
	}

	m_switch_value = !m_switch_value;

	if (-correct <= velocity_delta || speed <= 80.f)
	{
		if (m_switch_value)
		{
			yaw = yaw - ideal;
			cmd->csgo_user_cmd.base->sidemove = -1.0f;
			cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);

		}
		else
		{
			yaw = ideal + yaw;
			cmd->csgo_user_cmd.base->sidemove = 1.0f;
			cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);

		}
	}
	else
	{
		yaw = velocity_angle - correct;
		cmd->csgo_user_cmd.base->sidemove = 1.0f;
		cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);
	}

	rotate_movement(cmd, math::normalize_yaw(yaw));
}

void c_movement::limit_speed(c_user_cmd* cmd) 
{
	if (!g_cs2->local_player || !g_cs2->local_player->is_alive())
		return;

	if (!g_user->IsActive(xorstr_("ragebot_slowwalk"), xorstr_("ragebot_slowwalk"), 0))
		return;

	if (!g_cs2->local_player->get_weapon_active())
		return;

	auto weapon_data = g_cs2->local_player->get_weapon_active()->get_weapon_data();
	
	if (!weapon_data)
		return;

	float max_speed = weapon_data->max_speed() / 3.0f;


		const auto cmdSpeed{ std::sqrt((cmd->csgo_user_cmd.base->sidemove * cmd->csgo_user_cmd.base->sidemove) + (cmd->csgo_user_cmd.base->forwardmove * cmd->csgo_user_cmd.base->forwardmove)) };
	const auto velocity{ g_cs2->local_player->abs_velocity() };
	const auto speed{ velocity.length_2d() };

	if (cmdSpeed <= max_speed + 1.f
		&& velocity.length_2d() <= max_speed + 1.0f)
		return;

	vector forward{ }, right{ };
	math::angle_vectors(cmd->csgo_user_cmd.base->view->angles, &forward, &right, nullptr);

	const auto diff{ speed - max_speed };
	auto wishSpeed{ max_speed };

	vector velDir{ forward.x * cmd->csgo_user_cmd.base->forwardmove + right.x * cmd->csgo_user_cmd.base->sidemove,
		forward.y * cmd->csgo_user_cmd.base->forwardmove + right.y * cmd->csgo_user_cmd.base->sidemove, 0.0f };

	const auto accel{ g_cs2->sv_accelerate->GetValue<float>() };
	const auto maxAccelSpeed{ accel * 0.015625f * std::max(250.f, g_cs2->local_player->movement_services()->m_max_speed()) * g_cs2->local_player->movement_services()->m_flSurfaceFriction() };

	if (diff - maxAccelSpeed <= 0.f
		|| speed - maxAccelSpeed - 3.0f <= 0.0f)
		wishSpeed = max_speed;
	else {
		velDir = velocity;
		wishSpeed = -1.0f;
	}

	cmd->csgo_user_cmd.base->forwardmove = wishSpeed;
	cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_FORWARDMOVE);

	cmd->csgo_user_cmd.base->sidemove = 0.0f;
	cmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);

	auto moveDir{ cmd->csgo_user_cmd.base->view->angles.y };

	const auto direction{ std::atan2(velDir.y, velDir.x) };
	moveDir = std::remainderf(math::rad2deg(direction), 360.f);
}

void c_movement::fast_stop(c_user_cmd* m_pcmd)
{
	if (!g_cs2->local_player || !g_cs2->local_player->is_alive())
		return;

	if (!g_user->IsActive(xorstr_("misc_faststop"), xorstr_("misc_faststop"), 0))
		return;

	if (!(g_cs2->local_player->flags() & fl_onground && g_prediction_system->storage_.on_ground))
		return;

	auto pressed_move_key = m_pcmd->csgo_user_cmd.base->buttons->hold & IN_FORWARD || m_pcmd->csgo_user_cmd.base->buttons->hold & IN_MOVELEFT || m_pcmd->csgo_user_cmd.base->buttons->hold & IN_BACK || m_pcmd->csgo_user_cmd.base->buttons->hold & IN_MOVERIGHT || m_pcmd->csgo_user_cmd.base->buttons->hold & IN_JUMP;

	if (pressed_move_key)
		return;

	auto velocity = g_cs2->local_player->velocity();

	if (velocity.length_2d() > 5.0f)
	{
		vector direction;
		vector real_view;

		math::vector_angles(velocity, direction);
		real_view = Interfaces::csgo_input->GetViewAngles();

		direction.y = real_view.y - direction.y;

		vector forward;
		math::angle_vectors(direction, forward);

		static auto cl_forwardspeed = g_cs2->local_player->movement_services()->forward_move();
		static auto cl_sidespeed = g_cs2->local_player->movement_services()->side_move();

		auto negative_forward_speed = -g_cs2->local_player->movement_services()->forward_move();
		auto negative_side_speed = -g_cs2->local_player->movement_services()->side_move();

		auto negative_forward_direction = forward * negative_forward_speed;
		auto negative_side_direction = forward * negative_side_speed;

		m_pcmd->csgo_user_cmd.base->forwardmove = negative_forward_direction.x;
		m_pcmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_FORWARDMOVE);
		m_pcmd->csgo_user_cmd.base->sidemove = negative_side_direction.y;
		m_pcmd->csgo_user_cmd.base->set_bits(EBaseCmdBits::BASE_BITS_LEFTMOVE);
	}
}