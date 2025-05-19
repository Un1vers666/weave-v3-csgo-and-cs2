#pragma once

#include "../sdk.hpp"

class c_econ_entity : public c_base_model_entity
{
public:
	__forceinline c_attribute_container attribute_manager() {
		return *reinterpret_cast<c_attribute_container*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_AttributeManager);
	};

	__forceinline uint32_t original_owner_xuid_low() {
		return *reinterpret_cast<uint32_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_OriginalOwnerXuidLow);
	};
	__forceinline uint32_t original_owner_xuid_high() {
		return *reinterpret_cast<uint32_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_OriginalOwnerXuidHigh);
	};

	uint64_t original_owner_xuid() {
		return ((uint64_t)(original_owner_xuid_high()) << 32) | original_owner_xuid_low();
	}
};