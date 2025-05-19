#pragma once
#include "../../sdk/classes/user_cmd.hpp"

class c_movement
{
public:
	void bhop(c_user_cmd* cmd);
	void autostop();
	void auto_strafe(c_user_cmd* cmd);
	void limit_speed(c_user_cmd* cmd);
	void fast_stop(c_user_cmd* m_pcmd);
	void clamp_movement(c_user_cmd* cmd, float speed);
	void BunnyHop(c_user_cmd* pCmd, c_user_cmd_base* pUserCmd, c_player_movement_services* movement);
	void autopeek();
	void duck_assist();

	void on_pre(c_user_cmd* cmd);
	void on_post();

	struct {
		bool m_autopeeking;
		bool m_should_retrack;
		vector m_old_origin;
	} m_autopeek;

	struct {
		bool m_peeking;
	} m_duck_assist;

	struct {
		bool on_ground;
		bool should_check;
		bool subtick;
	} m_jump[150];

	bool is_subtick = false;
	bool should_fake = false;

	float  m_speed;
	float  m_ideal;
	float  m_ideal2;
	vector m_origin;
	bool  m_switch_value = false;
	int    m_strafe_index;
	float  m_old_yaw;
	float  m_circle_yaw;
	bool   m_invert;
	uint64_t last_pressed = 0;
	uint64_t last_buttons = 0;
};

inline auto g_movement = std::make_unique<c_movement>();