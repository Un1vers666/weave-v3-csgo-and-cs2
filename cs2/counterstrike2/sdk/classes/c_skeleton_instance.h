#pragma once

class c_cs_player_pawn;

class c_skeleton_instance
{
public:
	char pad_0001[0x1CC]; //0x0000
	int bone_count; //0x01BC 
	char pad_0002[0x18]; //0x01C0
	int bone_mask; //0x01D8 
	char pad_0003[0x4]; //0x01DC
	Matrix2x4_t* bone_cache;

	__forceinline c_model_state& m_modelState() {
		return *reinterpret_cast<c_model_state*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_modelState);
	};

	c_model* get_model()
	{
		auto ptr = *reinterpret_cast<void**>((uintptr_t)this + 0x200);

		if (!ptr)
			return nullptr;

		return *reinterpret_cast<c_model**>(ptr);
	}

	c_cs_player_pawn* get_owner_pawn()
	{
		return *reinterpret_cast<c_cs_player_pawn**>(std::uintptr_t(this) + 0x290);
	}

	void calc_world_space_bones(uint32_t bone_mask);
};