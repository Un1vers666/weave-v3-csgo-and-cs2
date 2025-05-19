// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../core.hpp"
#include "../gui.h"
#include "../../utilities/cloud/user.h"
#include "../utilities/hash.h"
#include <tinyformat/tinyformat.h>

void clip_board(std::string str)
{
	HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, str.size() + 1 * sizeof(CHAR));;
	LPVOID gl = GlobalLock(h);

	memcpy(gl, str.data(), str.size() * sizeof(CHAR));

	GlobalUnlock(gl);

	OpenClipboard(NULL); // hWnd is set to NULL
	EmptyClipboard(); // Sets clipboard ownership to someone don't know
	SetClipboardData(CF_TEXT, h);  // Successfully data to clipboard!
	CloseClipboard();
}

std::string get_time() {

	//const auto currentTime = std::chrono::system_clock::now();
	//auto unix_time = std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count();

	//const std::time_t epoch_plus_11h = 60 * 60 * 11;
	//const int local_time = localtime(&epoch_plus_11h)->tm_hour;
	//const int gm_time = gmtime(&epoch_plus_11h)->tm_hour;
	//const int tz_diff = local_time - gm_time;

	//UnixTime current_time_stamp(tz_diff);
	//current_time_stamp.getDateTime(unix_time);

	//UnixTime time_stamp(tz_diff);
	//auto p = std::to_string(g_user->subtime_);
	//p.pop_back();
	//p.pop_back();
	//p.pop_back();

	//time_stamp.getDateTime(atoi(p.c_str()));

	//if (std::fabsf(current_time_stamp.year - time_stamp.year) > 10)
	//	return xorstr_("Lifetime");

	//int days = int(time_stamp.month - current_time_stamp.month) * 30;
	//int current_days = days - (current_time_stamp.day - time_stamp.day);

	return std::to_string(14);
}

void c_gui::home_page()
{
	const render_position position = m_window_pos_;
	const render_color_multi gradient(render_color(255, 139, 59, 50), render_color(255, 139, 59, 50), render_color(14, 14, 16, 50), render_color(14, 14, 16, 50));
	const render_color_multi shadow(render_color(13, 13, 14, 0).override_alpha(g_render->get_override_alpha()), render_color(13, 13, 14, 0).override_alpha(g_render->get_override_alpha()), render_color(13, 13, 14, 255).override_alpha(g_render->get_override_alpha()), render_color(13, 13, 14, 255).override_alpha(g_render->get_override_alpha()));
	const render_position calc_text = g_render->get_font(2)->calc_text_size(13, FLT_MAX, FLT_MAX, g_user->GetUsername().c_str());

	std::uint64_t hash_search = std::hash<std::string>()(xorstr_("search"));
	std::uint64_t hash_configs = std::hash<std::string>()(xorstr_("configs"));

	begin_group(xorstr_("User Profile"), render_size(365, 102), false);
	{
		auto group_position = get_position();

		g_render->add_text(g_user->GetUsername(), group_position + render_position(60, 18), render_color(221, 221, 221), g_render->get_font(2), 13);
#ifdef _ALPHAD
		g_render->add_text(xorstr_("ALPHA"), group_position + render_position(60, 18) + render_position(calc_text.x + 5, 0), render_color(255, 139, 59), g_render->get_font(3), 13);
#elif _DEV
		g_render->add_text(xorstr_("DEV"), group_position + render_position(60, 18) + render_position(calc_text.x + 5, 0), render_color(255, 139, 59), g_render->get_font(3), 13);
#endif
		g_render->add_text(xorstr_("days left: ") + get_time(), group_position + render_position(60, 21 + g_render->get_font(2)->calc_text_size(13, FLT_MAX, FLT_MAX, g_user->GetUsername().c_str()).y), render_color(86, 86, 87), g_render->get_font(3), 11);
		g_render->add_texture_circle(group_position + render_position(14, 14), 17, g_user->GetAvatar(g_user->GetUsername()) == -1 ? 11 : g_user->GetAvatar(g_user->GetUsername()), true, render_color(255, 139, 59));
	}
	end_group();

	begin_group(xorstr_("Search"), render_size(365, 102), false);
	{

	}
	end_group();

	// scripts soon...
	{
		//render_position start = position + render_position(10, 177) + render_size(134, 0) + render_position(10, 0);
		//render_size size = render_size(90, 23);

		//bool is_hovered = helper.is_hovered(m_mouse_pos_, rect_(*reinterpret_cast<vec2_t*>(&start), *reinterpret_cast<vec2_t*>(&size)));

		//animation["button_scripts"].run(is_hovered);

		//g_render->rect_filled(position + render_position(10, 177) + render_size(134, 0) + render_position(10, 0), render_size(90, 23), render_color(255, 139, 59, 255 * animation["button_scripts"].base), 0, 10.f);
		//g_render->add_text("Scripts", position + render_position(10, 181) + render_size(134, 0) + render_position(10, 0) + render_position(30, 0), render_color(11, 11, 12), g_render->get_font(0), 12);
		//g_render->add_texture(position + render_position(10, 183) + render_size(134, 0) + render_position(10, 0) + render_position(14, 0), render_size(11, 11), 8);
	}

	render_position config_position = position + render_position(10, 210);
	auto config_padding = render_position(0, 0);
	int config_index = 0;

	animation[hash_configs].run(true, 0.6f);

	{
		bool has_active_rename = active_id == std::hash<std::string>()(xorstr_("rename_config"));

		animation[std::hash<std::string>()(xorstr_("rename_config_render"))].run(has_active_rename);

		render_position center = g_render->get_screen_size() / 2;
		auto size = render_size(200, 110);
		auto size_button = render_size(85, 25);

		g_render->push_override_buffer(2);

		g_render->rect_filled(center - (size * 0.5f), size, render_color(15, 15, 15).override_alpha(animation[std::hash<std::string>()(xorstr_("rename_config_render"))].base), 0, 10.f);

		g_render->rect_filled(center - (size * 0.5f) + render_position(10, size.y - 10 - size_button.y), size_button, render_color(27, 27, 27).override_alpha(animation[std::hash<std::string>()(xorstr_("rename_config_render"))].base), 0, 5.f);
		g_render->rect_filled(center - (size * 0.5f) + render_position(105, size.y - 10 - size_button.y), size_button, render_color(27, 27, 27).override_alpha(animation[std::hash<std::string>()(xorstr_("rename_config_render"))].base), 0, 5.f);

		auto calc_text_cancle = g_render->get_font(0)->calc_text_size(12, FLT_MAX, FLT_MAX, xorstr_("Cancel"));
		auto calc_text_save = g_render->get_font(0)->calc_text_size(12, FLT_MAX, FLT_MAX, xorstr_("Save"));

		g_render->add_text(xorstr_("Cancel"), center - (size * 0.5f) + render_position(10, size.y - 10 - size_button.y) + (size_button * 0.5f) - (calc_text_cancle * 0.5f), render_color::white().override_alpha(animation[std::hash<std::string>()(xorstr_("rename_config_render"))].base), g_render->get_font(0), 12);
		g_render->add_text(xorstr_("Save"), center - (size * 0.5f) + render_position(105, size.y - 10 - size_button.y) + (size_button * 0.5f) - (calc_text_save * 0.5f), render_color::white().override_alpha(animation[std::hash<std::string>()(xorstr_("rename_config_render"))].base), g_render->get_font(0), 12);

		//button cancle logic
		if (helper.is_hovered(m_mouse_pos_, rect_(center - (size * 0.5f) + render_position(10, size.y - 10 - size_button.y), size_button)) && has_pressed(VK_LBUTTON))
		{
			reset_active_id();
		}

		//button save logic
		if (helper.is_hovered(m_mouse_pos_, rect_(center - (size * 0.5f) + render_position(105, size.y - 10 - size_button.y), size_button)) && has_pressed(VK_LBUTTON) && !current_config.empty() && !input.empty())
		{
			std::thread([this]() {
				g_user->Processing(EProcessingType::Rename, ProcessingRecord(current_config, get_string_input(input), true));
				}).detach();
			g_user->GetConfigs()[current_config].name = get_string_input(input);
			reset_active_id();
		}


		g_render->rect_filled(center - (size * 0.5f) + render_position(10, 30), render_size(size.x - 20, 25), render_color(23, 23, 23).override_alpha(animation[std::hash<std::string>()(xorstr_("rename_config_render"))].base), 0, 5.f);

		if (helper.is_hovered(m_mouse_pos_, rect_(center - (size * 0.5f) + render_position(10, 30), render_size(size.x - 20, 25))) && has_pressed(VK_LBUTTON))
		{
			active_input = true;
		}

		if (active_input)
		{
			if (!g_gui->has_pressed(VK_RETURN))
			{
				if (g_gui->has_pressed(VK_BACK))
				{
					if (!input.empty())
						input.pop_back();
				}
				else if (input.size() < 16) {
					input += g_gui->input_character_queue;
				}
			}
		}

		g_render->add_text(get_string_input(input), center - (size * 0.5f) + render_position(15, 35), render_color::white().override_alpha(animation[std::hash<std::string>()(xorstr_("rename_config_render"))].base), g_render->get_font(0), 12);

		g_render->pop_override_buffer();
	}

	if (type_home_tab == 0) {

		std::wstring documentsPath = g_utils->get_documents_path();
		std::string directoryPath = g_utils->WStringToString(documentsPath) + "\\weave\\configs";
		g_user->LoadConfigsFromFiles(directoryPath, g_user->configs_);

		float max_size = std::ceil(float(g_user->GetConfigs().size()) / 4.f) * 75;
		max_size -= (75 * 3);
		max_size = std::max(0.f, max_size);

		g_user->mutex_.lock();
		g_render->set_limit(position + render_position(10, 210), render_size(750, 245));
		for (auto it = g_user->GetConfigs().begin(); it != g_user->GetConfigs().end();)
		{
			std::string config_uuid = xorstr_("config_") + std::to_string(config_index);
			std::uint64_t config_x = HASH_RT(tfm::format("%s_x", config_uuid).c_str());
			std::uint64_t config_y = HASH_RT(tfm::format("%s_y", config_uuid).c_str());
			std::uint64_t hash_config = std::hash<std::string>()(config_uuid);

			if (animation[hash_config].base <= 0.f && it->second.is_deleted)
			{
				it = g_user->GetConfigs().erase(it);
				continue;
			}

			if (animation_lerp[config_x] <= 0.f) {
				animation_lerp[config_x] = config_padding.x;
			}

			if (animation_lerp[config_y] <= 0.f) {
				animation_lerp[config_y] = config_padding.y;
			}

			animation_lerp[config_x] = std::lerp(animation_lerp[config_x], config_padding.x, 0.15f);
			animation_lerp[config_y] = std::lerp(animation_lerp[config_y], config_padding.y, 0.15f);

			config_position = position - render_position(0, m_scrollbar[FNV32("configs")].lerp_value) + render_position(10, 210) + render_position(animation_lerp[config_x], animation_lerp[config_y]);

			auto size = render_size(177, 75);

			auto active_clr = render_color(221, 221, 221);
			auto active2_clr = render_color(255, 139, 59);

			auto non_active_clr = render_color(86, 86, 87);

			bool is_hovered = helper.is_hovered(m_mouse_pos_, rect_(config_position, size)) && !has_active();

			bool has_active_or_hover = (!select_config.empty() && it->first.find(select_config) != std::string::npos) || is_hovered;
			animation[hash_config + HASH("act")].run(has_active_or_hover);

			animation[hash_config].run(!it->second.is_deleted);

			render_color name_clr = helper.animation_color(non_active_clr, active_clr, animation[hash_config + HASH("act")].base);
			render_color owner_clr = helper.animation_color(non_active_clr, active2_clr, animation[hash_config + HASH("act")].base);

			bool has_not_next_iterator = false;

			g_render->push_override_alpha(animation[hash_config].base);


			g_render->push_override_alpha(animation[hash_configs].base);
			g_render->set_limit(config_position, render_size(177, 75));
			g_render->rect_filled(config_position, render_size(177, 75), render_color(13, 13, 14), 0, 10.f);
			g_render->add_text(it->second.user, config_position + render_position(24, 21), owner_clr, g_render->get_font(1), 11);
			g_render->add_text(it->second.name, config_position + render_position(10, 5), name_clr, g_render->get_font(1), 12);
			g_render->rect_filled_multi(config_position + render_position(10, 5) + render_position(70, 0), render_size(40, 15), shadow);
			g_render->rect_filled(config_position + render_position(10, 5) + render_position(70, 0) + render_position(38, 0), render_size(90, 20), render_color(13, 13, 14).override_alpha(g_render->get_override_alpha()), 0, 10.f);
			g_render->add_text(it->second.updated, config_position + render_position(124, 6), render_color(86, 86, 87), g_render->get_font(1), 11);
			g_render->add_texture(config_position + render_position(10, 22), render_size(11, 11), g_gui->m_texture[4]);
			g_render->add_texture(config_position + render_position(10, 59), render_size(11, 11), g_gui->m_texture[2]);
			g_render->add_texture(config_position + render_position(26, 59), render_size(11, 11), g_gui->m_texture[3]);
			g_render->add_texture(config_position + render_position(42, 59), render_size(11, 11), g_gui->m_texture[5]);

			g_render->push_override_alpha(animation[hash_config + HASH("act")].base);
			g_render->arc_line(config_position + render_position(20, 10), 180, 90, 10, render_color(255, 139, 59));
			g_render->rect_filled_multi(config_position + render_position(0, 8), render_size(1, 30), render_color_multi(render_color(255, 139, 59), render_color(13, 13, 14), render_color(255, 139, 59), render_color(13, 13, 14)), 0);
			g_render->line(config_position + render_position(10, 0), config_position + render_position(10, 0) + render_size(20, 1), render_color(255, 139, 59), render_color(13, 13, 14));

			g_render->arc_line(config_position + render_position(177 - 21, 75 - 11), 0, 90, 10, render_color(255, 139, 59));
			g_render->rect_filled_multi(config_position + render_position(176, 45), render_size(1, 20), render_color_multi(render_color(13, 13, 14), render_color(255, 139, 59), render_color(13, 13, 14), render_color(255, 139, 59)), 0);
			g_render->line(config_position + render_position(167, 74), config_position + render_position(167, 74) + render_size(-30, 1), render_color(255, 139, 59), render_color(13, 13, 14));
			g_render->pop_override_alpha();

			g_render->pop_override_alpha();
			g_render->pop_override_alpha();
			g_render->pop_limit();

			if (helper.is_hovered(m_mouse_pos_, rect_(config_position + render_position(26, 59), render_size(11, 11))))
			{
				if (has_pressed(VK_LBUTTON)) {
					active_id = std::hash<std::string>()(xorstr_("rename_config"));
					input = get_wstring_input(g_user->GetConfigs()[it->first].name);
					current_config = it->second.share;
				}
			}
			else if (helper.is_hovered(m_mouse_pos_, rect_(config_position + render_position(42, 59), render_size(11, 11)))) {
				if (has_pressed(VK_LBUTTON) && !it->second.is_deleted) {
					std::thread([this, it]() {
						g_user->Processing(EProcessingType::Delete, ProcessingRecord(it->first, true));
						}).detach();
				}
			}
			else if (helper.is_hovered(m_mouse_pos_, rect_(config_position + render_position(10, 59), render_size(11, 11)))) {
				if (has_pressed(VK_LBUTTON) && it->second.available_share) {
					clip_board(it->first);
				}
			}
			else if (is_hovered) {
				if (has_pressed(VK_LBUTTON)) {
					if (!select_config.empty() && it->first.find(select_config) != std::string::npos) {
						std::thread([this, it]() {
							g_user->Processing(EProcessingType::Save, ProcessingRecord(it->first, true));
							}).detach();
					}
					else {
						select_config = it->first;
						std::thread([this]() {
							g_user->Processing(EProcessingType::Load, ProcessingRecord(select_config, true));
							}).detach();
					}
				}
			}

			if (int(config_index % 4) == 3)
			{
				config_padding.x = 0;
				config_padding += render_position(0, 75);
				config_padding += render_position(0, 10);
			}
			else
			{
				config_padding += render_position(177, 0);
				config_padding += render_position(10, 0);
			}

			config_index++;
			if (!has_not_next_iterator)
				it++;
		}

		g_user->mutex_.unlock();

		if (animation_lerp[HASH("create_config_x")] <= 0.f) {
			animation_lerp[HASH("create_config_x")] = 10 + config_padding.x;
		}

		if (animation_lerp[HASH("create_config_y")] <= 0.f) {
			animation_lerp[HASH("create_config_y")] = 210 + config_padding.y;
		}

		animation_lerp[HASH("create_config_x")] = std::lerp(animation_lerp[HASH("create_config_x")], 10 + config_padding.x, 0.15f);
		animation_lerp[HASH("create_config_y")] = std::lerp(animation_lerp[HASH("create_config_y")], 210 + config_padding.y, 0.15f);

		{
			config_position = position - render_position(0, m_scrollbar[FNV32("configs")].lerp_value) + render_position(animation_lerp[HASH("create_config_x")], animation_lerp[HASH("create_config_y")]);

			auto size = render_size(177, 75);

			auto active_clr = render_color::white();

			auto non_active_clr = render_color(86, 86, 87);

			std::string config_uuid = xorstr_("config_") + std::to_string(config_index);
			std::uint64_t hash_config = std::hash<std::string>()(config_uuid);

			bool is_hovered = helper.is_hovered(m_mouse_pos_, rect_(config_position, size)) && !has_active();

			bool has_active_or_hover = is_hovered;
			animation[hash_config].run(has_active_or_hover);

			render_color cross_clr = helper.animation_color(non_active_clr, active_clr, animation[hash_config].base);

			bool has_not_next_iterator = false;


			g_render->push_override_alpha(animation[hash_configs].base);
			g_render->set_limit(config_position, render_size(177, 75));
			g_render->rect_filled(config_position, render_size(177, 75), render_color(13, 13, 14), 0, 10.f);
			g_render->rect_filled_multi(config_position + render_position(10, 5) + render_position(70, 0), render_size(40, 15), shadow);
			g_render->rect_filled(config_position + render_position(10, 5) + render_position(70, 0) + render_position(38, 0), render_size(90, 20), render_color(13, 13, 14).override_alpha(g_render->get_override_alpha()), 0, 10.f);
			auto center = config_position + (render_size(177, 75) / 2);
			auto size_cross = render_size(23, 4);
			g_render->pop_limit();
			g_render->add_texture(center - render_size(15, 15), render_size(30, 30), 9, cross_clr);
			g_render->pop_override_alpha();

			if (is_hovered) {
				if (has_pressed(VK_LBUTTON)) {
					std::thread([]() {
						g_user->Processing(EProcessingType::Create);
						}).detach();
				}
			}
		}

		if (g_input_system->get_scroll() != 0 && g_gui->m_hovered_window == FNV32("Menu"))
		{
			m_scrollbar[FNV32("configs")].value += 85 * -g_input_system->get_scroll();
			m_scrollbar[FNV32("configs")].value = std::max(0.f, m_scrollbar[FNV32("configs")].value);
			m_scrollbar[FNV32("configs")].value = std::clamp(m_scrollbar[FNV32("configs")].value, 0.f, max_size);
			g_input_system->reset_scrolling();
		}

		m_scrollbar[FNV32("configs")].update();

		g_render->pop_limit();

	}
	else if (type_home_tab == 1) {
		g_user->mutex_.lock();

		float max_size = std::ceil(float(g_user->GetOnline().size()) / 4.f) * 75;
		max_size -= (75 * 3);
		max_size = std::max(0.f, max_size);

		g_render->set_limit(position + render_position(10, 210), render_size(window_size.x, 85 * 3));

		for (auto it = g_user->GetOnline().begin(); it != g_user->GetOnline().end();)
		{
			config_position = position - render_position(0, m_scrollbar[FNV32("users")].lerp_value) + render_position(10, 210) + config_padding;

			auto size = render_size(177, 75);

			auto active_clr = render_color(221, 221, 221);
			auto active2_clr = render_color(255, 139, 59);

			auto non_active_clr = render_color(86, 86, 87);

			std::string config_uuid = xorstr_("user_") + std::to_string(config_index);
			std::uint64_t hash_config = std::hash<std::string>()(config_uuid);

			bool is_hovered = helper.is_hovered(m_mouse_pos_, rect_(config_position, size)) && !has_active();

			bool has_active_or_hover = is_hovered;
			animation[hash_config].run(has_active_or_hover);

			render_color name_clr = helper.animation_color(non_active_clr, active_clr, animation[hash_config].base);
			render_color owner_clr = helper.animation_color(non_active_clr, active2_clr, animation[hash_config].base);

			bool has_not_next_iterator = false;

			g_render->push_override_alpha(animation[hash_configs].base);
			g_render->rect_filled(config_position, render_size(177, 75), render_color(13, 13, 14), 0, 10.f);
			g_render->set_limit(config_position + render_position(14, 14), render_size(34, 34));
			g_render->add_texture_circle(config_position + render_position(14, 14), 17, g_user->GetAvatar(it->username) == -1 ? 11 : g_user->GetAvatar(it->username));
			g_render->pop_limit();
			g_render->add_text(it->username, config_position + render_position(60, 18), name_clr, g_render->get_font(2), 13);
			g_render->pop_override_alpha();

			if (int(config_index % 4) == 3)
			{
				config_padding.x = 0;
				config_padding += render_position(0, 75);
				config_padding += render_position(0, 10);
			}
			else
			{
				config_padding += render_position(177, 0);
				config_padding += render_position(10, 0);
			}

			config_index++;
			if (!has_not_next_iterator)
				it++;
		}

		g_render->pop_limit();

		if (g_input_system->get_scroll() != 0 && g_gui->m_hovered_window == FNV32("Menu"))
		{
			m_scrollbar[FNV32("users")].value += 75 * -g_input_system->get_scroll();
			m_scrollbar[FNV32("users")].value = std::max(0.f, m_scrollbar[FNV32("users")].value);
			m_scrollbar[FNV32("users")].value = std::clamp(m_scrollbar[FNV32("users")].value, 0.f, max_size);
			g_input_system->reset_scrolling();
		}

		m_scrollbar[FNV32("users")].update();

		g_user->mutex_.unlock();
	}


	{
		render_position start = position + render_position(399, 114);
		auto size = render_size(337, 39);

		if (!active_search && animation[hash_search].base <= 0.0f)
			search.clear();

		animation[hash_search].run(active_search, 0.25f);

		g_render->set_limit(position + render_position(399, 114), render_size(337, 45 + (70 * 3) * animation[hash_search].base));

		g_render->rect_filled(position + render_position(399, 114) + render_position(0, 150) - render_position(0, 150 * animation[hash_search].base), render_size(337, 45 + (70 * 3)), render_color(17, 17, 17, 255 * animation[hash_search].base), 0, 10);

		if (active_search)
		{
			if (!g_gui->has_pressed(VK_RETURN))
			{
				if (g_gui->has_pressed(VK_BACK))
				{
					if (!search.empty())
						search.pop_back();
				}
				else if (search.size() < 24) {
					search += g_gui->input_character_queue;
				}
			}
		}

		int find_element = 0;

		if (m_scroll.find(std::hash<std::string>()(xorstr_("search"))) == m_scroll.end())
			m_scroll.emplace(std::hash<std::string>()(xorstr_("search")), gui_scroll());

		auto& scroll_info = m_scroll.find(std::hash<std::string>()(xorstr_("search")))->second;

		render_position element_padding = render_position();

		auto search_lower = get_string_input(search);
		std::transform(search_lower.begin(), search_lower.end(), search_lower.begin(), [](unsigned char c) { return std::tolower(c); });

		for (auto& element : elements)
		{
			auto& name_lower = element.second.name_;
			std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), [](unsigned char c) { return std::tolower(c); });
			if (!search.empty())
			{
				if (name_lower.find(search_lower) == std::string::npos)
					continue;
			}

			if (active_id != hash_search)
				continue;

			std::string p = element.second.tab_ + xorstr_(" -> ") + element.second.group_;
			render_position element_position = position + render_position(399, 114) + render_position(16, 49) + element_padding - render_position(0, scroll_info.animation_element_scroll);
			g_render->rect_filled(element_position, render_size(303, 59), render_color(13, 13, 14, 255 * animation[hash_search].base), 0, 10);
			g_render->add_text(p, element_position + render_position(14, 10), render_color(86, 86, 87, 255 * animation[hash_search].base), g_render->get_font(2), 13);
			g_render->add_text(element.second.name_, element_position + render_position(14, 33), render_color(221, 221, 221, 255 * animation[hash_search].base), g_render->get_font(2), 13);

			if (helper.is_hovered(m_mouse_pos_, { element_position ,render_size(303, 59) }) && !helper.is_hovered(m_mouse_pos_, { position + render_position(399, 114) , render_size(337,45) }) && has_pressed(VK_LBUTTON))
			{
				anim_active_id = element.second.tab;
				reset_active_id();
				active_search = false;
				break;
			}

			find_element++;
			element_padding += render_position(0, 59 + 10);
		}

		g_render->pop_limit();

		if (!find_element)
			find_element = elements.size();

		int perfect_elements = std::clamp(find_element - 3, 0, int(elements.size()));
		int max_visible = 3;

		if (find_element > 3)
		{

			float percent = (3.f / find_element);


			float scroll = g_input_system->get_scroll();

			float max_size = 195.f - ((65 * 3) * percent);

			float max_size2 = 69.f * perfect_elements;

			if (scroll > 0.f && g_gui->m_hovered_window == FNV32("Menu"))
			{
				scroll_info.scroll_amount -= max_size / perfect_elements;
				scroll_info.scroll_element_amount -= 69.f;
			}
			else if (scroll < 0.f && g_gui->m_hovered_window == FNV32("Menu"))
			{
				scroll_info.scroll_amount += max_size / perfect_elements;
				scroll_info.scroll_element_amount += 69.f;
			}

			scroll_info.scroll_amount = std::clamp(scroll_info.scroll_amount, 0.f, max_size);
			scroll_info.scroll_element_amount = std::clamp(scroll_info.scroll_element_amount, 0.f, max_size2);

			float delta = scroll_info.scroll_amount - scroll_info.animation_scroll;

			scroll_info.animation_scroll += (delta * 0.05f);

			scroll_info.animation_scroll = std::clamp(scroll_info.animation_scroll, 0.f, max_size);

			float delta2 = scroll_info.scroll_element_amount - scroll_info.animation_element_scroll;

			scroll_info.animation_element_scroll += (delta2 * 0.05f);

			scroll_info.animation_element_scroll = std::clamp(scroll_info.animation_element_scroll, 0.f, max_size2);

			g_render->rect_filled(position + render_position(399, 114) + render_position(327, 50), render_size(4, (65 * 3)), render_color(13, 13, 14, 255 * animation[hash_search].base), 0, 0);
			g_render->rect_filled(position + render_position(399, 114) + render_position(327, 50 + scroll_info.animation_scroll), render_size(4, (65 * 3) * percent), render_color(255, 139, 59, 255 * animation[hash_search].base), 0, 0);

			if (g_gui->m_last_moved_window == FNV32("Menu"))
				g_input_system->reset_scrolling();
		}

		auto is_hovered_search = helper.is_hovered(m_pressed_mouse_pos_, rect_(start, render_size(337, 45 + (70 * 3) * active_search)));

		if (is_hovered_search && has_pressed(VK_LBUTTON) && active_id == -1)
		{
			active_id = hash_search;
			active_search = true;
		}

		if (!is_hovered_search && active_search && has_pressed(VK_LBUTTON) && active_id == hash_search)
		{
			active_id = -1;
			active_search = false;
		}

		g_render->rect_filled(position + render_position(399, 114), render_size(337, 39), render_color(23, 23, 23), 0, 10.f);

		std::string penis = xorstr_("Type something...");

		if (active_search)
			penis = get_string_input(search);

		if (m_input_text.find(hash_search) == m_input_text.end())
			m_input_text.emplace(hash_search, gui_text_input());

		auto& input_text_info = m_input_text.find(hash_search)->second;

		auto delta_text = g_render->get_font(2)->calc_text_size(13, FLT_MAX, FLT_MAX, penis.c_str()).x;

		float text = delta_text;

		if (active_search)
		{
			delta_text = g_render->get_font(2)->calc_text_size(13, FLT_MAX, FLT_MAX, penis.c_str()).x - input_text_info.animation_text;
			input_text_info.animation_text += delta_text * 0.08f;
			text = input_text_info.animation_text;

			if (input_text_info.animation_.base == 0.0f)
				input_text_info.active_ = true;
			else if (input_text_info.animation_.base >= 1.0f)
				input_text_info.active_ = false;

			input_text_info.animation_.run(input_text_info.active_, 0.7f);
		}
		else
		{
			input_text_info.active_ = false;
			input_text_info.animation_.run(input_text_info.active_, 0.1f);
		}

		g_render->set_limit(position + render_position(442, 126), render_position(text, g_render->get_font(2)->calc_text_size(13, FLT_MAX, FLT_MAX, penis.c_str()).y + 4));
		g_render->add_text(penis, position + render_position(442, 126), render_color(86, 86, 87), g_render->get_font(2), 13);
		g_render->pop_limit();
		g_render->rect_filled(position + render_position(442, 126) + render_position(text, g_render->get_font(2)->calc_text_size(13, FLT_MAX, FLT_MAX, penis.c_str()).y) + render_position(2, 0), render_size(5, 1), render_color(100, 100, 100).override_alpha(input_text_info.animation_.base));
		g_render->add_texture(position + render_position(415, 124), render_size(20, 20), g_gui->m_texture[6]);
	}

	g_render->rect_filled(position + render_position(10, 177), render_size(134, 23), render_color(255, 139, 59), 0, 10.f);
	g_render->add_text(xorstr_("Configuration"), position + render_position(40, 181), render_color(11, 11, 12), g_render->get_font(0), 12);
	g_render->add_texture(position + render_position(24, 183), render_size(11, 11), g_gui->m_texture[7]);

	g_render->rect_filled(position + render_position(149, 177), render_size(60, 23), render_color(255, 139, 59), 0, 10.f);
	g_render->add_text(xorstr_("Chat"), position + render_position(164, 181), render_color(11, 11, 12), g_render->get_font(0), 12);

	if (helper.is_hovered(m_pressed_mouse_pos_, rect_(position + render_position(149, 177), render_size(60, 23))) && has_pressed(VK_LBUTTON) && !g_cs2->m_open_chat) {
		g_cs2->m_open_chat = true;
		m_last_moved_window = FNV32("Chat");
	}

	g_render->rect_filled(position + render_position(214, 177), render_size(60, 23), render_color(255, 139, 59), 0, 10.f);
	g_render->add_text(xorstr_("Users"), position + render_position(225, 181), render_color(11, 11, 12), g_render->get_font(0), 12);

	if (helper.is_hovered(m_pressed_mouse_pos_, rect_(position + render_position(214, 177), render_size(60, 23))) && has_pressed(VK_LBUTTON)) {
		std::thread t([]() {
			g_user->Processing(EProcessingType::GetOnline);
			});
		t.detach();
		type_home_tab = 1;
	}

	if (helper.is_hovered(m_pressed_mouse_pos_, rect_(position + render_position(10, 177), render_size(134, 23))) && has_pressed(VK_LBUTTON)) {
		type_home_tab = 0;
	}
}