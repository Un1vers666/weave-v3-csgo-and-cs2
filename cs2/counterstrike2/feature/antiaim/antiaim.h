#pragma once

#include "../../sdk/classes/user_cmd.hpp"
#include "../../sdk/interfaces/i_csgo_input.hpp"
#include "../../sdk/sdk.hpp"
#include "../../sdk/math/vector.hpp"

#include "../../sdk/math/math.hpp"

class c_antiaim
{
	enum e_manual
	{
		none,
		left,
		backward,
		right
	};

	enum
	{
		SIDE_NONE = -1,
		SIDE_BACK,
		SIDE_LEFT,
		SIDE_RIGHT
	};

	enum JitterType {
		JITTER_TYPE_NONE = 0,
		JITTER_TYPE_CENTER,
		JITTER_TYPE_3_WAY,
		JITTER_TYPE_RANDOM,
		JITTER_TYPE_SPIN,
	};
public:
	void freestanding(c_user_cmd* m_pcmd);
	void run(c_user_cmd* cmd);
private:
	void pitch(c_user_cmd* cmd);
	void yaw(c_user_cmd* cmd);
public:
	float spinner_amount = 0.f;
	int manual = 0;
	vector antiaim_angle{};
	int jitter_side = -1;
	int final_manual_side = -1;
};

inline auto g_antiaim = std::make_unique<c_antiaim>();