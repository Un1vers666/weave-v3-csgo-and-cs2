// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "visuals.hpp"

#include <tinyformat/tinyformat.h>

class CMapInfo
{
public:
	char pad[0x4B4];
	float bomb_radius;
};

void c_visuals::render_text(render_position pos, render_color clr, const char* txt, render_font* font = nullptr, int size = 11, bool outline = false)
{
	g_render->get_buffer()->text(font, size, txt, pos, clr.override_alpha(g_render->get_override_alpha()), outline);
}

float get_damage_bombe(c_planted* c4)
{
	float radius = g_cs2->bomb_radius.at(g_cs2->map) * 3.5f;

	auto vec_to_target = reinterpret_cast<c_cs_player_pawn*>(c4)->game_scene_node()->abs_origin() - g_cs2->local_player->game_scene_node()->abs_origin();

	float fGaussianFalloff = exp(vec_to_target.length_sqr() * (-1.5 / ((radius * 0.33333334f) * radius)));

	return fGaussianFalloff * g_cs2->bomb_radius.at(g_cs2->map);
}

void c_visuals::run_c4_esp(c_planted* c4)
{
	vector box = { };

	auto handle = c_handle(c4->handle_bomb_defuser());

	defuser_bomb_pawn = Interfaces::entity_list->get_entity(handle.get_index());
	damage = get_damage_bombe(c4);
	timer = c4->blow() - Interfaces::globals->global_storage.curtime;

	if (timer > 0.f)
	{
		auto result = g_visuals->get_bounding_box(
			reinterpret_cast<c_cs_player_pawn*>(c4)->game_scene_node()->abs_origin(),
			reinterpret_cast<c_cs_player_pawn*>(c4)->collision_property()->mins(),
			reinterpret_cast<c_cs_player_pawn*>(c4)->collision_property()->maxs()
		);

		if (result.first)
		{
			auto ret = result.second;

			auto center = ret.x + (ret.w * 0.5f);

			if (defuser_bomb_pawn)
			{
				float def_left = c4->defuse_count_down() - Interfaces::globals->global_storage.curtime;
				std::string text = tfm::format("bomb (%.2fs) (%.2f) (defuse left: %.2f)", timer, damage, def_left);
				auto calc_text = g_render->get_font(6)->calc_text_size(10, FLT_MAX, FLT_MAX, text.c_str());

				render_text(render_position(center - (calc_text.x / 2), ret.y + ret.h + 1),
					render_color(255, 255, 255, 255), text.c_str(), g_render->get_font(6), 10, true);
			}
			else
			{
				std::string text = tfm::format("bomb (%.2fs) (%.2f)", timer, damage);

				auto calc_text = g_render->get_font(6)->calc_text_size(10, FLT_MAX, FLT_MAX, text.c_str());
				render_text(render_position(center - (calc_text.x / 2), ret.y + ret.h + 1),
					render_color(255, 255, 255, 255), text.c_str(), g_render->get_font(6), 10, true);
			}
		}
	}
}