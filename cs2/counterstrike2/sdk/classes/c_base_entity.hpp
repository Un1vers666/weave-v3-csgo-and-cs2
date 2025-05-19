#pragma once

#include "../sdk.hpp"

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define ENT_ENTRY_MASK 0x7FFF
#define NUM_SERIAL_NUM_SHIFT_BITS 15

class c_game_scene_node;

class c_main_class_information
{
public:
	const char* designer_class;
	const char* class_;
	const char* name_class;
	void* unk;
	c_main_class_information* next_class;
};

class c_second_class_information
{
public:
	char pad[0x28];
	c_main_class_information* main_class_information;
};

class c_unkown_information
{
public:
	char pad[0x8];
	c_second_class_information* second_class_information;
	char pad2[0x10];
	const char* first_name;
	char pad3[0x10];
	int number;
};

class c_entity_identity
{
public:
	__forceinline int get_flags() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flags);
	};

	__forceinline const char* get_designer_name() {
		return *reinterpret_cast<const char**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_designerName);
	};

	__forceinline std::uint32_t get_index() {
		return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint64_t>(this) + 0x10);
	};

	bool is_valid()
	{
		return get_index() != INVALID_EHANDLE_INDEX;
	}

	int get_entry_index()
	{
		if (!is_valid())
			return ENT_ENTRY_MASK;

		return get_index() & ENT_ENTRY_MASK;
	}

	int get_serial_number()
	{
		return get_index() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}


};

class c_entity_instance
{
public:
	__forceinline c_entity_identity* identity() {
		return *reinterpret_cast<c_entity_identity**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pEntity);
	};

	__forceinline c_base_handle get_ref_handle()
	{
		c_entity_identity* pIdentity = identity();
		if (pIdentity == nullptr)
			return c_base_handle();

		return c_base_handle(pIdentity->get_entry_index(), pIdentity->get_serial_number() - (pIdentity->get_flags() & 1));
	}

	std::uint32_t get_entity_handle();
};

class c_base_entity : public c_entity_instance
{
public:
	int health() {
		return *(int*)(std::uintptr_t(this) + g_pSchemaOffset->m_iHealth);
	}
	__forceinline int life_state() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_lifeState);
	};
	__forceinline int eflags() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iEFlags);
	};
	__forceinline int team_num() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iTeamNum);
	};
	__forceinline int flags() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_fFlags);
	};
	__forceinline void set_flags(int new_flags) {
		*reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_fFlags) = new_flags;
	}
	__forceinline float speed() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flSpeed);
	};
	__forceinline vector velocity() {
		return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_vecVelocity);
	};
	__forceinline vector abs_velocity() {
		return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_vecAbsVelocity);
	};
	__forceinline movetype_t move_type() {
		return *reinterpret_cast<movetype_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_MoveType);
	};
	__forceinline movetype_t actual_move_type() {
		return *reinterpret_cast<movetype_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nActualMoveType);
	};
	__forceinline c_game_scene_node* game_scene_node() {
		return *reinterpret_cast<c_game_scene_node**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pGameSceneNode);
	};
	__forceinline c_collision_property* collision_property() {
		return *reinterpret_cast<c_collision_property**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pCollision);
	};
	__forceinline c_handle owner_handle() {
		return *reinterpret_cast<c_handle*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_hOwnerEntity);
	};
	__forceinline c_handle ground_entity() {
		return *reinterpret_cast<c_handle*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_hGroundEntity);
	};
	__forceinline float sim_time() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flSimulationTime);
	};
	__forceinline void set_sim_time(float time) {
		*reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flSimulationTime) = time;
	}
	__forceinline std::uint64_t& subclass_id() {
		return *reinterpret_cast<std::uint64_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nSubclassID);
	};
public:
	bool is_alive()
	{
		return health() > 0;
	}

	bool is_base_player_weapon()
	{
		return CALL_VIRTUAL(bool, 156, this);
	}
public:
	c_skeleton_instance* get_skeleton()
	{
		return CALL_VIRTUAL(c_skeleton_instance*, 8, this);
		//return *reinterpret_cast<c_skeleton_instance**>(reinterpret_cast<std::uintptr_t>(this) + 0x318);
	}

	c_body_component_base_anim_graph* get_body_base_anim_graph()
	{
		return *reinterpret_cast<c_body_component_base_anim_graph**>(std::uintptr_t(this) + 0x38);
	}

	void set_model(const char* name);
	void set_scene_origin(vector origin);
	void update_subclass();

	const char* get_class_name()
	{
		c_unkown_information* info = get_class_information();

		if (!info->second_class_information || !info->second_class_information->main_class_information)
			return "";

		return info->second_class_information->main_class_information->name_class;
	}

	bool find_class(std::uint64_t name)
	{
		c_unkown_information* info = get_class_information();

		if (!info->second_class_information || !info->second_class_information->main_class_information)
			return false;

		for (auto _class = info->second_class_information->main_class_information; _class->next_class != nullptr; _class = _class->next_class)
		{
			if (HASH_RT(_class->name_class) == name)
				return true;
		}

		return false;
	}

	c_unkown_information* get_class_information() {
		return *(c_unkown_information**)(std::uintptr_t(this) + 0x10);
	}

	bool is_player() {
		return find_class(HASH("C_CSPlayerPawn"));
	}

	bool is_controller() {
		return find_class(HASH("CCSPlayerController"));
	}
};