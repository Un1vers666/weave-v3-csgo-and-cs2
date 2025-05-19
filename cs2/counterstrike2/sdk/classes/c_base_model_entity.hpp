#pragma once

#include "../sdk.hpp"

class c_base_entity;

class c_glow_property {
public:
	__forceinline vector glow_color() {
		return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_fGlowColor);
	};

	__forceinline int glow_type() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iGlowType);
	};

	__forceinline int glow_team() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iGlowTeam);
	};

	__forceinline int glow_range() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nGlowRange);
	};

	__forceinline int glow_min_range() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nGlowRangeMin);
	};

	__forceinline color_t glow_overide_color() {
		return *reinterpret_cast<color_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_glowColorOverride);
	};

	__forceinline bool glow_flashing() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bFlashing);
	};

	__forceinline float glow_time() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flGlowTime);
	};

	__forceinline float glow_star_time() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flGlowStartTime);
	};

	__forceinline bool eligible_for_screen_highlight() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bEligibleForScreenHighlight);
	};

	__forceinline bool glowing() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bGlowing);
	};

	void set_glowing_true() {
		// Compute the address of the `glowing` bool
		bool* glowingPtr = reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bGlowing);

		// Set the value to `true`
		*glowingPtr = true;
	}

	__forceinline c_entity_instance* get_owner() {
		return *reinterpret_cast<c_entity_instance**>(reinterpret_cast<std::uint64_t>(this) + 0x18);
	};
};

class c_base_model_entity : public c_base_entity
{
public:
    __forceinline color_t render_color() {
        return *reinterpret_cast<color_t*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nRenderFX);
    };
    __forceinline vector view_offset() {
        return *reinterpret_cast<vector*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_vecViewOffset);
    };
    __forceinline c_client_alpha_property* client_alpha_property() {
        return *reinterpret_cast<c_client_alpha_property**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_pClientAlphaProperty);
    };
public:
    bool is_view_model() { return CALL_VIRTUAL(bool, 241, this); }
};