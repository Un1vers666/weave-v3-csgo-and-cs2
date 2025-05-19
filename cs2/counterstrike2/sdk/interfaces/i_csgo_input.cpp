#include "i_csgo_input.hpp"
#include "../sdk/sdk.hpp"

vector i_csgo_input::GetViewAngles()
{
	using fnGetViewAngles = std::int64_t(__fastcall*)(i_csgo_input*, std::int32_t);
	return *reinterpret_cast<vector*>(g_cs2->m_patterns[FNV32("get_view_angles")].as<fnGetViewAngles>()(this, 0));
}

void i_csgo_input::SetViewAngle(vector& angView)
{
	using fnSetViewAngle = std::int64_t(__fastcall*)(void*, std::int32_t, vector&);
	*reinterpret_cast<vector*>(g_cs2->m_patterns[FNV32("set_view_angles")].as<fnSetViewAngle>()(this, 0, std::ref(angView)));
}

void i_csgo_input::add_button(int button) {
    const auto buttonState{ Interfaces::get_button_state(this, 0) };

    auto& buttonCount{ recast_at_offset(int, buttonState, 0x34) };

    auto currentButton{ (BYTE*)(reinterpret_cast<std::uintptr_t>(buttonState) + 24 * (3 + buttonCount)) };

    auto& v15{ recast_at_offset(__int64, currentButton, -8) };
    v15 = button;
    *currentButton |= button;

    buttonCount += 1;
}

void i_csgo_input::remove_button(int button) {
	const auto buttonState{ Interfaces::get_button_state(this, 0) };

	auto& buttonCount{ recast_at_offset(int, buttonState, 0x34) };

	auto v6{ recast_at_offset(int, this, 0x54F8) };

	while ((int)v6 < buttonCount) {
		auto currentButton{ (BYTE*)(reinterpret_cast<std::uintptr_t>(buttonState) + 24 * (3 + v6)) };

		auto& v15{ recast_at_offset(__int64, currentButton, -8) };
		if (v15 & button)
			v15 = 0;

		++v6;
	}
}
