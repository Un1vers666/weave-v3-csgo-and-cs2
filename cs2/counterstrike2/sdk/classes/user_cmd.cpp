// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

void c_user_cmd::set_button_state(std::uint64_t button, EInButtonState call_type)
{
	auto v7 = ~button;

	if (call_type == EInButtonState::IN_BUTTON_DOWN)
	{
		buttons.hold = buttons.hold & v7 | button;
		csgo_user_cmd.base->buttons->hold = csgo_user_cmd.base->buttons->hold & v7 | button;
	}

	if (call_type == EInButtonState::IN_BUTTON_DOWN_UP)
	{
		buttons.toggle = buttons.toggle & v7 | button;
		csgo_user_cmd.base->buttons->toggle = csgo_user_cmd.base->buttons->toggle & v7 | button;
	}

	if (call_type == EInButtonState::IN_BUTTON_UP_DOWN_UP)
	{
		buttons.scroll = buttons.scroll & v7 | button;
		csgo_user_cmd.base->buttons->scroll = csgo_user_cmd.base->buttons->scroll & v7 | button;
	}
}

void c_user_cmd::remove_button_state(std::uint64_t button)
{
	buttons.hold &= ~button;
	buttons.toggle &= ~button;
}

void c_user_cmd_base::Set(int button, bool pressed)
{
    //bool once = pressed;

    //for (int i{}; i < subtick_move.size; i++)
    //{
    //    auto subtick = subtick_move.Get(i);

    //    if (!subtick)
    //        continue;

    //    subtick->buttons = button;
    //    subtick->pressed = true;
    //    subtick->when = 0.865f;
    //}
}

void c_user_cmd_base::UnSet(int button, bool pressed)
{
    //for (int i{}; i < subtick_move.size; i++)
    //{
    //    auto subtick = subtick_move.Get(i);

    //    if (!subtick)
    //        continue;

    //    subtick->buttons = 0;
    //    subtick->pressed = false;
    //}
}

int c_user_cmd_base::get_cache_size()
{
	return CALL_VIRTUAL(int, 7, this);
}

void c_user_cmd::set_attack_history(int index) {
	csgo_user_cmd.attack_history_index[0] = index;
	csgo_user_cmd.set_bits(ECSGOUserCmdBits::CSGOUSERCMD_BITS_ATTACK1START);
	csgo_user_cmd.attack_history_index[1] = index;
	csgo_user_cmd.set_bits(ECSGOUserCmdBits::CSGOUSERCMD_BITS_ATTACK2START);
	csgo_user_cmd.attack_history_index[2] = index;
	csgo_user_cmd.set_bits(ECSGOUserCmdBits::CSGOUSERCMD_BITS_ATTACK3START);
}

c_subtick_move_step* c_user_cmd::create_subtick_step(void* arena) {
	return g_cs2->m_patterns[FNV32("create_sub_tick")].as<c_subtick_move_step * (__fastcall*)(void*)>()(arena);
}

c_subtick_move_step* c_user_cmd::create_subtick() {

	if (!csgo_user_cmd.base)
		return nullptr;

	if (csgo_user_cmd.base->subtick_move.m_pRep && csgo_user_cmd.base->subtick_move.m_nCurrentSize < csgo_user_cmd.base->subtick_move.m_pRep->m_nAllocatedSize) {
		return csgo_user_cmd.base->subtick_move.m_pRep->m_tElements[csgo_user_cmd.base->subtick_move.m_nCurrentSize++];
	}

	auto new_subtick = create_subtick_step(csgo_user_cmd.base->subtick_move.m_pArena);

	return g_cs2->m_patterns[FNV32("add_sub_tick")].as<c_subtick_move_step * (__fastcall*)(RepeatedPtrField_t<c_subtick_move_step>&, c_subtick_move_step*)>()(csgo_user_cmd.base->subtick_move, new_subtick);
}

void c_user_cmd::add_subtick(bool pressed, int button, bool needs_delete) {
	auto subtick = create_subtick();

	if (!subtick)
		return;

	subtick->m_has_bits = 7;
	subtick->buttons = button;
	subtick->pressed = pressed;
	subtick->unk = 4;
	subtick->when = 0.999f;
}

void c_user_cmd::set_input_history_angle(vector& angles)
{
	if (!csgo_user_cmd.input_history.m_pRep)
		return;

	for (std::int32_t i = 0; i < csgo_user_cmd.input_history.m_nCurrentSize; i++)
	{
		csgo_input_history_entry* tick = csgo_user_cmd.input_history.m_pRep->m_tElements[i];

		if (tick && tick->angle)
		{
			tick->angle->angles = angles;
			tick->set_bits(EInputHistoryBits::INPUT_HISTORY_BITS_VIEWANGLES);
		}
	}
}

void c_user_cmd::override_angle(const vector& angView)
{
	if (!csgo_user_cmd.input_history.m_pRep)
		return;

	for (std::int32_t i = 0; i < csgo_user_cmd.input_history.m_nCurrentSize; i++)
	{
		csgo_input_history_entry* tick = csgo_user_cmd.input_history.m_pRep->m_tElements[i];
		if (tick == nullptr)
			continue;

		if (tick->angle == nullptr) {

			tick->angle = tick->create_msg_angle();
			tick->set_bits(EInputHistoryBits::INPUT_HISTORY_BITS_VIEWANGLES);

			if (!tick->angle)
				continue;
		}

		tick->angle->angles = angView;
		tick->set_bits(EInputHistoryBits::INPUT_HISTORY_BITS_VIEWANGLES);

	}
}

csgo_interpolation_info_pb* csgo_input_history_entry::create_new_interp()
{
	this->m_cached_bits |= 1;

	std::uint64_t a1 = static_cast<std::uint64_t>(m_has_bits & 0xFFFFFFFFFFFFFFFC);

	if ((m_has_bits & 1) != 0)
		a1 = a1;

	using fn = csgo_interpolation_info_pb * (__fastcall*)(std::uint64_t);

	static auto fnCreateInterp = g_cs2->m_patterns[FNV32("create_interp")].get_absolute_address(1, 0).cast<fn>();
	return fnCreateInterp(a1);
}

c_cmd_qangle* csgo_input_history_entry::create_msg_angle()
{
	this->m_cached_bits |= 1;

	std::uint64_t a1 = static_cast<std::uint64_t>(m_has_bits & 0xFFFFFFFFFFFFFFFC);

	if ((m_has_bits & 1) != 0)
		a1 = a1;

	// Define the function type correctly
	using fn = c_cmd_qangle * (__fastcall*)(std::uint64_t);

	// Retrieve the function pointer, cast it to the correct type, and then call it
	static auto fncreate = reinterpret_cast<fn>(g_cs2->m_patterns[FNV32("create_cmd_angle")].get());
	return fncreate(a1);
}
