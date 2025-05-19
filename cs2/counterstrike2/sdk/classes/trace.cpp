// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "trace.hpp"
#include "../sdk.hpp"

c_trace_filter::c_trace_filter(std::uint64_t trace_mask, c_cs_player_pawn* local_player, c_cs_player_pawn* entity, std::uint8_t layer)
{
	this->ptr = g_cs2->m_patterns[FNV32("trace_filter")].cast<std::uintptr_t*>();
	this->trace_mask = trace_mask;

	if (local_player)
	{
		this->skip_handle1 = g_cs2->get_handle_entity(local_player);
		this->skip_owner_handle1 = g_cs2->get_handle_entity(Interfaces::entity_list->get_entity(local_player->owner_handle().get_index()));
		this->collision1 = g_cs2->get_colision_entity(local_player);
	}
	else
	{
		this->skip_handle1 = -1;
		this->collision1 = -1;
		this->skip_owner_handle1 = -1;
	}

	if (entity)
	{
		this->skip_handle2 = g_cs2->get_handle_entity(entity);
		this->skip_owner_handle2 = g_cs2->get_handle_entity(Interfaces::entity_list->get_entity(entity->owner_handle().get_index()));
		this->collision2 = g_cs2->get_colision_entity(entity);
	}
	else
	{
		this->skip_handle2 = -1;
		this->collision2 = -1;
		this->skip_owner_handle2 = -1;
	}

	this->N0000011C = 7;

	this->layer = layer;

	this->flags = 0x49;

	this->null_it3 = 0;
	this->unk1 = 0;
	this->unk2 = 0;
}

c_surfaces_data* c_game_trace::get_surfaces_data()
{
	using function_t = c_surfaces_data*(__fastcall*)(void*);

	if (!this->surfaces)
		return nullptr;

	return g_cs2->m_patterns[FNV32("get_surface_data")].as<function_t>()(this->surfaces);
}