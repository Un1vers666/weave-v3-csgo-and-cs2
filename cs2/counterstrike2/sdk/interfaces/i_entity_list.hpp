#pragma once

#include "../../signatures.hpp"
#include "../../utilities/utilities.hpp"

#include "../classes/c_entity2_network_classes.hpp"

class c_cs_player_pawn;
class c_cs_player_controller;

class i_entity_list
{
public:

	/// GetClientEntity
	template <typename T = c_base_entity>
	T* Get(int nIndex)
	{
		return reinterpret_cast<T*>(this->GetEntityByIndex(nIndex));
	}

	/// GetClientEntityFromHandle
	template <typename T = c_base_entity>
	T* Get(c_handle hHandle)
	{
		if (!hHandle.is_valid())
			return nullptr;

		return reinterpret_cast<T*>(this->GetEntityByIndex(hHandle.get_index()));
	}

	template <typename T = c_base_entity>
	T* Get(c_base_handle hHandle)
	{
		if (!hHandle.IsValid())
			return nullptr;

		return reinterpret_cast<T*>(this->GetEntityByIndex(hHandle.GetEntryIndex()));
	}

	c_cs_player_pawn* get_entity(int index)
	{
		__int64 v2; // rcx
		__int64 v3; // r8
		__int64 result{}; // rax

		if ((unsigned int)index <= 0x7FFE
			&& (unsigned int)(index >> 9) <= 0x3F
			&& (v2 = *(std::uintptr_t*)(std::uintptr_t(this) + 8 * (index >> 9) + 16)) != 0
			&& (v3 = 120 * (index & 0x1FF), v3 + v2)
			&& (*(std::uintptr_t*)(v3 + v2 + 16) & 0x7FFF) == index)
		{
			result = *(std::uintptr_t*)(v3 + v2);
		}
		return reinterpret_cast<c_cs_player_pawn*>(result);
	}

	c_cs_player_pawn* get_split_screen_view_player(int index);

	c_cs_player_controller* get_controller(int index)
	{
		return reinterpret_cast<c_cs_player_controller*>(get_entity(index));
	}

	int get_highest_entity_index()
	{
		return *(int*)(std::uintptr_t(this) + 0x1510);
	}

	std::uintptr_t* get_entity2_save_restore()
	{
		return *(std::uintptr_t**)(std::uintptr_t(this) + 0x1560);
	}

	c_entity2_network_classes* get_entity2_network_classes()
	{
		return *(c_entity2_network_classes**)(std::uintptr_t(this) + 0x1568);
	}
private:
	void* GetEntityByIndex(int nIndex);

	c_cs_player_pawn* get_local_player();
};