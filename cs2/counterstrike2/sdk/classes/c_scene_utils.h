#pragma once

#include "../interfaces/virtual.hpp"

class c_scene_utils
{
public:
	std::uintptr_t* get_unk(std::uint64_t seed)
	{
		return CALL_VIRTUAL(std::uintptr_t*, 25, this, seed);
	}

	c_material_system2_app_system_dict* get_material_system2_app_system_dict() {
		auto material_app = *reinterpret_cast<uintptr_t**>(uintptr_t(this) + 0xF8);
		return reinterpret_cast<c_material_system2_app_system_dict*>(uintptr_t(material_app) + 0x4F0);
	}
};