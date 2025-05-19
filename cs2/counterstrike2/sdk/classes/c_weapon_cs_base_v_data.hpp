#pragma once

#include "../sdk.hpp"

class firing_mode_float
{
public:
	float value[2];
};

class c_weapon_cs_base_v_data
{
public:
	__forceinline int weapon_type() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_WeaponType);
	};
	__forceinline bool is_revolver() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bIsRevolver);
	};
	__forceinline const char*& name() {
		return *reinterpret_cast<const char**>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_szName);
	};
	__forceinline bool has_burst_mode() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bHasBurstMode);
	};
	__forceinline float max_speed() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flMaxSpeed);
	};
	__forceinline float spread() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flSpread);
	};
	__forceinline int recoil_seed() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nRecoilSeed);
	};
	__forceinline int spread_seed() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nSpreadSeed);
	};
	__forceinline int damage() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nDamage);
	};
	__forceinline float headshot_multiplier() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flHeadshotMultiplier);
	};
	__forceinline float armor_ratio() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flArmorRatio);
	};
	__forceinline float penetration() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flPenetration);
	};
	__forceinline float range() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flRange);
	};
	__forceinline float range_modifier() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flRangeModifier);
	};
	__forceinline int bullets() {
		return *reinterpret_cast<int*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_nNumBullets);
	};
	__forceinline float throw_velocity() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flThrowVelocity);
	}

	__forceinline float cycle_time() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_flCycleTime);
	}

	__forceinline float max_clip() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_iMaxClip1);
	}

	__forceinline float crouch_inaccuracy() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + 0xc64);
	}

	__forceinline float stand_inaccuracy() {
		return *reinterpret_cast<float*>(reinterpret_cast<std::uint64_t>(this) + 0xc6c);
	}
};