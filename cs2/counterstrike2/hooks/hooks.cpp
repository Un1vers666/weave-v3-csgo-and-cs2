// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "hooks.hpp"
#include "../Logging.h"
#include <tinyformat/tinyformat.h>
#include "../utilities/exception/weave_error.h"

#define add_hook(pattern, function, original) if (MH_CreateHook(pattern.as<void*>(), &function, reinterpret_cast<void**>(&original)) != MH_OK) \
												{ \
													weave_error(xorstr_(#function));\
													return false; \
												} \

#define init_hook() 	if (MH_Initialize() != MH_OK) \
	{ \
		weave_error(xorstr_("failed to initialize minhook")); \
		return false; \
	} \

#define enable_hooks() 	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) \
	{ \
		weave_error(xorstr_("Failed Hook Enable")); \
		return false; \
	} \

bool hooks::render_initilization()
{
	Interfaces::m_scene_utils = Interfaces::get_interface<c_scene_utils>(std::uint64_t(GetModuleHandleA("scenesystem.dll")), xorstr_("SceneUtils_001"));
	auto swap_chain = Interfaces::m_scene_utils->get_material_system2_app_system_dict()->swap_chains->swap_chain_dx->swap_chain;
	g_cs2->m_patterns[FNV32("present")] = std::uintptr_t(g_utils->get_virtual(swap_chain, 8));
	g_cs2->m_patterns[FNV32("resize_bufferes")] = std::uintptr_t(g_utils->get_virtual(swap_chain, 13));

	init_hook();

	add_hook(g_cs2->m_patterns[FNV32("present")], hooks::swap_chain_present::hook, swap_chain_present_original);
	add_hook(g_cs2->m_patterns[FNV32("resize_bufferes")], hooks::swap_chain_resize_buffers::hook, swap_chain_resize_buffers_original);

	enable_hooks();

	return true;
}

extern void __fastcall PanoramaIcon(const char* str, std::uintptr_t* aye);

decltype(&PanoramaIcon) original_panorama_icon;

//E8 ?? ?? ?? ?? 48 8B 4F 18 48 8D 53 20
void PanoramaIcon(const char* str, std::uintptr_t* aye)
{
	g_cs2->m_latest_hook = LoggerHack::PanoramaImage;
	if(str && !strcmp(str, xorstr_("econ/status_icons/trophy_majors_finalists")))
		return original_panorama_icon(xorstr_("icons/xp/level2234"), aye);

	return original_panorama_icon(str, aye);
}

void __fastcall input_history(c_input_message* input_message, csgo_input_history_entry* history, bool has_attack, std::uint64_t a4, std::uint64_t a5, std::uint64_t a6)
{
	g_cs2->m_latest_hook = LoggerHack::InputHistory;
	if(!g_cs2->local_player || !g_cs2->local_player->is_alive())
		return  original_input_history(input_message, history, has_attack, a4, a5, a6);

	g_cs2->render_fraction = input_message->render_fraction;
	g_cs2->render_tick = input_message->render_tick_count;
	g_cs2->player_tick = input_message->player_tick_count;
	g_cs2->player_fraction = input_message->player_fraction;

	//if (!g_ragebot->valid_interpolation_timing())
	//	return original_input_history(input_message, history, has_attack, a4, a5, a6);

	if (input_message)
	{
		if (Interfaces::engine->is_connected() || Interfaces::engine->is_in_game())
		{
			if (g_ragebot->sub_tick_data.msg == g_ragebot->msg_t::lag_compensation)
			{
				history->render_tick_count = g_ragebot->sub_tick_data.best_tick;
				history->m_cached_bits |= 0x800u;

				if (history->cl_interp) {
					history->cl_interp->m_cached_bits |= 2u;
					auto cl_cachedbits = history->cl_interp->m_cached_bits;
					history->cl_interp->src_tick = g_ragebot->sub_tick_data.best_tick;
					cl_cachedbits |= 4;
					history->cl_interp->m_cached_bits = cl_cachedbits;
					history->cl_interp->dst_tick = g_ragebot->sub_tick_data.best_tick + 1;
					history->cl_interp->m_cached_bits = cl_cachedbits | 1;
					//history->cl_interp->m_flFraction = fraction;
					history->cl_interp->m_cached_bits |= 0x40u;

					//history->cl_interp->src_tick = g_ragebot->sub_tick_data.best_tick + 1 /*cl_interp.nSrcTick*/;
					//history->cl_interp->dst_tick = g_ragebot->sub_tick_data.best_tick /*cl_interp.nDstTick*/;
					//tick->cl_interp->fraction = cl_interp.flFraction;
					//history->set_bits(EInputHistoryBits::INPUT_HISTORY_BITS_CL_INTERP);
				}

				if (history->sv_interp0) {
					history->sv_interp0->m_cached_bits |= 2u;
					auto sv_cachedbits_0 = history->sv_interp0->m_cached_bits;
					history->sv_interp0->src_tick = g_ragebot->sub_tick_data.best_tick;
					sv_cachedbits_0 |= 4;
					history->sv_interp0->m_cached_bits = sv_cachedbits_0;
					history->sv_interp0->dst_tick = g_ragebot->sub_tick_data.best_tick + 1;
					history->sv_interp0->m_cached_bits = sv_cachedbits_0 | 1;
					//history->sv_interp0->m_flFraction = 0.f;
					history->sv_interp0->m_cached_bits |= 0x80u;

					//history->sv_interp0->src_tick = g_ragebot->sub_tick_data.best_tick + 1 /*sv_interp0.nSrcTick*/;
					//history->sv_interp0->dst_tick = g_ragebot->sub_tick_data.best_tick /*sv_interp0.nDstTick*/;
					//tick->sv_interp0->fraction = sv_interp0.flFraction;
					//history->set_bits(EInputHistoryBits::INPUT_HISTORY_BITS_SV_INTERP0);
				}

				if (history->sv_interp1) {
					history->sv_interp1->m_cached_bits |= 2u;
					auto pl_cachedbits_1 = history->sv_interp1->m_cached_bits;
					history->sv_interp1->src_tick = g_ragebot->sub_tick_data.best_tick + 1;
					pl_cachedbits_1 |= 4;
					history->sv_interp1->m_cached_bits = pl_cachedbits_1;
					history->sv_interp1->dst_tick = g_ragebot->sub_tick_data.best_tick + 2;
					history->sv_interp1->m_cached_bits = pl_cachedbits_1 | 1;
					//history->sv_interp1->m_flFraction = 0.f;

					//history->sv_interp1->src_tick = g_ragebot->sub_tick_data.best_tick + 1 /*sv_interp1.nSrcTick*/;
					//history->sv_interp1->dst_tick = g_ragebot->sub_tick_data.best_tick /*sv_interp1.nDstTick*/;
					//tick->sv_interp1->fraction = sv_interp1.flFraction;
					//history->set_bits(EInputHistoryBits::INPUT_HISTORY_BITS_SV_INTERP1);
				}

				if (history->player_interp) {
					history->player_interp->m_cached_bits |= 2u;
					auto sv_cachedbits_1 = history->player_interp->m_cached_bits;
					history->player_interp->src_tick = g_ragebot->sub_tick_data.best_tick + 1;
					sv_cachedbits_1 |= 4;
					history->player_interp->m_cached_bits = sv_cachedbits_1;
					history->player_interp->dst_tick = g_ragebot->sub_tick_data.best_tick + 2;
					history->player_interp->m_cached_bits = sv_cachedbits_1 | 1;

					//history->player_interp->src_tick = g_ragebot->sub_tick_data.best_tick + 1;
					//history->player_interp->dst_tick = g_ragebot->sub_tick_data.best_tick;
					//history->set_bits(EInputHistoryBits::INPUT_HISTORY_BITS_PLAYER_INTERP);
				}

				g_ragebot->sub_tick_data.msg = g_ragebot->msg_t::invalid;
			}

			if (g_ragebot->sub_tick_data.command == g_ragebot->command_msg::silent) {
				// modify predicted sub tick viewangles to our rage bestpoint forcing silent w predicted cmds
				input_message->angle = g_ragebot->sub_tick_data.best_point_vec;

				history->angle->angles = g_ragebot->sub_tick_data.best_point;
				g_ragebot->sub_tick_data.response = g_ragebot->response_msg::validated_view_angles;

			}
			else
				g_ragebot->sub_tick_data.response = g_ragebot->response_msg::empty;
		}
	}
	
	//if(has_attack)
	//	original_input_history(input_message, history, has_attack, a4, a5, a6);

	return original_input_history(input_message, history, true, a4, a5, a6);
}

extern void __fastcall render_scope_shader(std::uintptr_t* a1, std::uintptr_t* a2);

decltype(&render_scope_shader) original_is_render_scope_shader;

void __fastcall render_scope_shader(std::uintptr_t* a1, std::uintptr_t* a2) {
	g_cs2->m_latest_hook = LoggerHack::RenderScope;
	if (g_user->IsActive(xorstr_("misc_remove_scope"), xorstr_("misc_disable_light"), 2))
		return;

	return original_is_render_scope_shader(a1, a2);
}

extern void __fastcall calculation_view_bob(std::uintptr_t* a1, std::uintptr_t* a2, float* a3, float* a4);

decltype(&calculation_view_bob) original_calculation_view_bob;

void __fastcall calculation_view_bob(std::uintptr_t* a1, std::uintptr_t* a2, float* a3, float* a4) {
	g_cs2->m_latest_hook = LoggerHack::CalculationViewBob;
	if (g_user->IsActive(xorstr_("misc_remove_bob"), xorstr_("misc_disable_light"), 6))
		return;

	return original_calculation_view_bob(a1, a2, a3, a4);
}

extern void __fastcall world_render_(std::uintptr_t* a1, int* a2, std::uintptr_t* a3, std::uintptr_t* a4);

decltype(&world_render_) original_world_render;

void __fastcall world_render_(std::uintptr_t* a1, int* a2, std::uintptr_t* a3, std::uintptr_t* a4) {

	if(!Interfaces::engine->is_connected() || !Interfaces::engine->is_in_game())
		return original_world_render(a1, a2, a3, a4);

	return original_world_render(a1, a2, a3, a4);
}

extern std::uintptr_t* __fastcall animation_fix(std::uintptr_t* a1, std::uintptr_t* a2, std::uintptr_t* a3);

decltype(&animation_fix) original_animation_fix;

std::uintptr_t* __fastcall animation_fix(std::uintptr_t* a1, std::uintptr_t* a2, std::uintptr_t* a3) {
	g_cs2->m_latest_hook = LoggerHack::AnimationFix;
	const char* is_knife = *(const char**)(std::uintptr_t(a1) + 0x8);
	if (strstr(is_knife, xorstr_("weapons/models/knife")) != nullptr)
		*(std::uintptr_t**)(std::uintptr_t(a3) + 0x30) = nullptr;
	return original_animation_fix(a1, a2, a3);
}


struct penis_cvar {
	char* name;
};

struct aye_convar {
	char pad[0x8];
	penis_cvar* p_cvar;
};


extern void* __fastcall get_cvar(aye_convar* cvar, int flags);
decltype(&get_cvar) original_get_cvar;

unsigned long long get_hash(char* txt) {
	return HASH_RT(txt);
}

int* SetCvar(aye_convar* cvar, int flags) {

	if (!cvar->p_cvar)
		return nullptr;

	if (get_hash(cvar->p_cvar->name) == HASH("lb_enable_lights") && g_cs2->m_disable_light)
	{
		int value = 0;
		return &value;
	}

	return nullptr;
}

void* __fastcall get_cvar(aye_convar* cvar, int flags) {

	g_cs2->m_latest_hook = LoggerHack::GetCvarValue;

	if(!cvar)
		return original_get_cvar(cvar, flags);

	int* cvar_ = SetCvar(cvar, flags);

	if(cvar_ == nullptr)
		return original_get_cvar(cvar, flags);

	return cvar_;
}

extern __int64 __fastcall server_seed_hash(vector angle, int tick);

decltype(&server_seed_hash) original_server_seed_hash;

__int64 __fastcall server_seed_hash(vector angle, int tick) {
	LogInfo(std::to_string(tick));
	return original_server_seed_hash(angle, tick);
}

extern void AggregateP(std::uintptr_t* a1, std::uintptr_t* a2, std::uintptr_t* a3, std::uintptr_t* a4);
decltype(&AggregateP) original_aggregate;
void AggregateP(std::uintptr_t* a1, std::uintptr_t* a2, std::uintptr_t* a3, std::uintptr_t* a4) {
	return original_aggregate(a1, a2, a3, a4);
}

extern void* __fastcall lag_comp(void* subTickData, void* inputHistoryFieldCurrent, char a3, __int64 a4, __int64 a5, __int64 a6);
decltype(&lag_comp) original_lag_comp;
void* __fastcall lag_comp(void* subTickData, void* inputHistoryFieldCurrent, char a3, __int64 a4, __int64 a5, __int64 a6) {
	const auto og{ original_lag_comp };

	auto std{ ((c_input_message*)subTickData) };
	auto ihe{ ((csgo_input_history_entry*)inputHistoryFieldCurrent) };

	g_cs2->render_fraction = std->render_fraction;
	g_cs2->render_tick = std->render_tick_count;
	g_cs2->player_tick = std->player_tick_count;
	g_cs2->player_fraction = std->player_fraction;

	return og(subTickData, inputHistoryFieldCurrent, a3, a4, a5, a6);
}

extern void* glow_property(c_glow_property* glow_property);
decltype(&glow_property) original_glow_property;
void* glow_property(c_glow_property* glow_property) {

	void* result = original_glow_property(glow_property);

	if (!Interfaces::engine->is_connected() || !Interfaces::engine->is_in_game())
		return result;

	c_entity_instance* glow_player_instance = glow_property->get_owner();
	if (glow_player_instance == nullptr) {
		return result;
	}

	c_cs_player_pawn* glow_player = reinterpret_cast<c_cs_player_pawn*>(glow_player_instance);
	if (glow_player == nullptr || glow_player == g_cs2->local_player || !glow_player->is_alive()) {
		return result;
	}
	color_t targetColor{ 0, 0, 0 };
	if (glow_player->is_enemy() /*&& g_cfg.m_visuals->m_team_glow*/) {
		glow_property->glow_overide_color() = targetColor; /*g_cfg.m_visuals->m_team_glow_color*/;
		glow_property->set_glowing_true();
	}

	return result;
}

extern bool __fastcall should_grenade_crosshair(c_base_cs_grenade* grenade);
decltype(&should_grenade_crosshair) original_should_grenade_crosshair;
bool __fastcall should_grenade_crosshair(c_base_cs_grenade* grenade) {
	g_cs2->m_latest_hook = LoggerHack::ShouldGrenadeCrosshair;
	if (g_user->IsActive(xorstr_("misc_remove_grenade_crosshair"), xorstr_("misc_remove_grenade_crosshair"), 0))
		return false;

	return original_should_grenade_crosshair(grenade);
}

extern void* __fastcall on_add_entity(void* rcx, c_entity_instance* pInstance, c_base_handle hHandle);
decltype(&on_add_entity) original_on_add_entity;
void* __fastcall on_add_entity(void* rcx, c_entity_instance* pInstance, c_base_handle hHandle) {
	EntCache::OnAddEntity(pInstance, hHandle);

	return original_on_add_entity(rcx, pInstance, hHandle);
}

extern void* __fastcall on_remove_entity(void* rcx, c_entity_instance* pInstance, c_base_handle hHandle);
decltype(&on_remove_entity) original_on_remove_entity;
void* __fastcall on_remove_entity(void* rcx, c_entity_instance* pInstance, c_base_handle hHandle) {

	EntCache::OnRemoveEntity(pInstance, hHandle);

	return original_on_remove_entity(rcx, pInstance, hHandle);
}

bool hooks::initialize()
{
	add_trace(__PRETTY_FUNCTION__);

	g_cs2->m_patterns[FNV32("relative_mode_mouse")] = std::uintptr_t(g_utils->get_virtual(Interfaces::input_system, 78));
	g_cs2->m_patterns[FNV32("kukold_move")] = std::uintptr_t(g_utils->get_virtual(Interfaces::csgo_input, 7));
	g_cs2->m_patterns[FNV32("frame_stage_notify")] = std::uintptr_t(g_utils->get_virtual(Interfaces::client, 36));
	g_cs2->m_patterns[FNV32("create_move")] = std::uintptr_t(g_utils->get_virtual(Interfaces::csgo_input, 5));
	g_cs2->m_patterns[FNV32("on_add_entity")] = std::uintptr_t(g_utils->get_virtual(Interfaces::entity_list, 14));
	g_cs2->m_patterns[FNV32("on_remove_entity")] = std::uintptr_t(g_utils->get_virtual(Interfaces::entity_list, 15));

	//add_hook(g_cs2->m_patterns[FNV32("on_add_entity")], on_add_entity, original_on_add_entity);
	//add_hook(g_cs2->m_patterns[FNV32("on_remove_entity")], on_remove_entity, original_on_remove_entity);
	//
	//EntCache::CacheCurrentEntities();

	add_hook(g_cs2->m_patterns[FNV32("should_grenade_crosshair")], should_grenade_crosshair, original_should_grenade_crosshair);
	//add_hook(g_cs2->m_patterns[FNV32("run_simulation")], hooks::run_simulation::hook, run_simulation_original);
	add_hook(g_cs2->m_patterns[FNV32("get_cvar_value")], get_cvar, original_get_cvar);
	add_hook(g_cs2->m_patterns[FNV32("animation_fix")], animation_fix, original_animation_fix);
	add_hook(g_cs2->m_patterns[FNV32("calculation_view_bob")], calculation_view_bob, original_calculation_view_bob);
	add_hook(g_cs2->m_patterns[FNV32("render_scope")], render_scope_shader, original_is_render_scope_shader);
	add_hook(g_cs2->m_patterns[FNV32("kukold_move")], hooks::verify_input::hook, verify_input_original);
	add_hook(g_cs2->m_patterns[FNV32("setup_view")], hooks::setup_view::hook, setup_view_original);
	add_hook(g_cs2->m_patterns[FNV32("calc_viewmodel_view")], hooks::calc_viewmodel_view::hook, calc_viewmodel_view_original);
	add_hook(g_cs2->m_patterns[FNV32("on_render_start")], hooks::on_render_start::hook, on_render_start_original);
	add_hook(g_cs2->m_patterns[FNV32("draw_crosshair")], hooks::draw_crosshair::hook, draw_crosshair_original);
	//add_hook(g_cs2->m_patterns[FNV32("debug_info")], hooks::debug_info::hook, debug_info_original);
	add_hook(g_cs2->m_patterns[FNV32("set_function_bool")], hooks::set_function_bool::hook, set_function_bool_original);
	add_hook(g_cs2->m_patterns[FNV32("set_function_vector")], hooks::set_function_vector::hook, set_function_vector_original);
	add_hook(g_cs2->m_patterns[FNV32("get_fov")], hooks::get_fov::hook, get_fov_original);
	add_hook(g_cs2->m_patterns[FNV32("get_fov2")], hooks::get_fov2::hook, get_fov2_original);
	add_hook(g_cs2->m_patterns[FNV32("run_command")], hooks::run_command::hook, run_command_original);
	add_hook(g_cs2->m_patterns[FNV32("smoke_vortex_draw")], hooks::smoke_voxel_draw::hook, smoke_voxel_draw_original);
	add_hook(g_cs2->m_patterns[FNV32("frame_stage_notify")], hooks::frame_stage_notify::hook, frame_stage_notify_original);
	add_hook(g_cs2->m_patterns[FNV32("create_move")], hooks::create_move::hook, create_move_original);
	add_hook(g_cs2->m_patterns[FNV32("level_init")], hooks::level_init::hook, level_init_original);
	add_hook(g_cs2->m_patterns[FNV32("level_shutdown")], hooks::level_shutdown::hook, level_shutdown_original);
	add_hook(g_cs2->m_patterns[FNV32("override_view")], hooks::override_view::hook, override_view_original);
	add_hook(g_cs2->m_patterns[FNV32("draw_scene_object")], hooks::draw_scene_object::hook, draw_scene_object_original);
	add_hook(g_cs2->m_patterns[FNV32("relative_mode_mouse")], hooks::relative_mouse_mode::hook, relative_mouse_mode_original);
	add_hook(g_cs2->m_patterns[FNV32("send_move_internal")], hooks::send_move_internal::hook, send_move_internal_original);
	add_hook(g_cs2->m_patterns[FNV32("grenade_tracer")], hooks::grenade_tracer::hook, grenade_tracer_original);
	add_hook(g_cs2->m_patterns[FNV32("draw_processing")], hooks::draw_processing::hook, draw_processing_original);
	//add_hook(g_cs2->m_patterns[FNV32("processing")], hooks::post_processing::hook, post_processing_original);
	add_hook(g_cs2->m_patterns[FNV32("show_legs")], hooks::show_legs::hook, show_legs_original);
	add_hook(g_cs2->m_patterns[FNV32("set_model")], hooks::set_model::hook, set_model_original);
	add_hook(g_cs2->m_patterns[FNV32("is_loadout_allowed")], hooks::is_loadout_allowed::hook, is_loadout_allowed_original);
	add_hook(g_cs2->m_patterns[FNV32("set_thirdperson_name_tag")], hooks::local_thirdperson_name::hook, third_person_name_original);
	//add_hook(g_cs2->m_patterns[FNV32("merge_bones")], hooks::merge_bones::hook, merge_bone_original);
	add_hook(g_cs2->m_patterns[FNV32("set_light_color")].get_absolute_address(1, 0), hooks::set_lighting_color::hook, set_lighting_color_original);
	//add_hook(g_cs2->m_patterns[FNV32("popup_accept_match_found")], hooks::get_popup::hook, get_popup_original);
	add_hook(g_cs2->m_patterns[FNV32("recview_message")], hooks::recview_message::hook, recview_message_original);
	//add_hook(g_cs2->m_patterns[FNV32("glow_property")], glow_property, original_glow_property);
	add_hook(g_cs2->m_patterns[FNV32("panorama_icon")], PanoramaIcon, original_panorama_icon);
	add_hook(g_cs2->m_patterns[FNV32("input_history")], input_history, original_input_history);
	//add_hook(sign, world_render_, original_world_render);

	enable_hooks();

	LogInfo("hooks initialized");

	//lagcomp::g_lagcomp->Initialize();

	g_event->initilization();

	LogInfo("event manager initialized");
	return true;
}

void hooks::release()
{
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
}

static c_bone_data frozenBoneMatrix[28]{};

c_bone_data sub(c_bone_data data, c_cs_player_pawn* pawn) {
	c_bone_data d{};
	d.scale = data.scale;
	d.rot = data.rot;
	d.pos = data.pos - pawn->game_scene_node()->abs_origin();
	return d;
}

c_bone_data add(c_bone_data data, c_cs_player_pawn* pawn) {
	c_bone_data d{};
	d.scale = data.scale;
	d.rot = data.rot;
	d.pos = data.pos + pawn->game_scene_node()->abs_origin();
	return d;
}

void __fastcall hooks::merge_bones::hook(c_skeleton_instance* a1, uint32_t a2)
{
	static float lastUpdateTime = 0.f;

	if (!a1 || !a1->m_modelState().bones || !a1->m_modelState().m_hModel()->hitbox_number() /*|| !g_user->IsActive(xorstr_("ragebot_slowwalk"), xorstr_("ragebot_slowwalk"), 0)*/)
		return merge_bone_original(a1, a2);

	if (!g_cs2->local_player || !g_cs2->local_player->is_alive())
		return merge_bone_original(a1, a2);

	if (a1 != g_cs2->local_player->game_scene_node()->get_skeleton_instance())
		return merge_bone_original(a1, a2);

	if (lastUpdateTime > Interfaces::globals->global_storage.curtime)
		lastUpdateTime = 0.f;

	float freezeTime = 250.f / 1000.f;
	if (Interfaces::globals->global_storage.curtime - lastUpdateTime >= freezeTime) {
		lastUpdateTime = Interfaces::globals->global_storage.curtime;

		merge_bone_original(a1, a2);

		for (int i = 0; i <= 27; i++)
			frozenBoneMatrix[i] = sub(a1->m_modelState().bones[i], g_cs2->local_player);
		return;
	}

	merge_bone_original(a1, a2);

	for (int i = 0; i <= 27; i++)
		a1->m_modelState().bones[i] = add(frozenBoneMatrix[i], g_cs2->local_player);
}
