#pragma once

#include "../sdk.hpp"

class c_networked_sequence_operation
{
public:
	int sequence() {
		return *(int*)(std::uintptr_t(this) + g_pSchemaOffset->m_hSequence);
	}
	float prev_cycle() {
		return *(float*)(std::uintptr_t(this) + g_pSchemaOffset->m_flPrevCycle);
	}
	float cycle() {
		return *(float*)(std::uintptr_t(this) + g_pSchemaOffset->m_flCycle);
	}
	int weight() {
		return *(int*)(std::uintptr_t(this) + g_pSchemaOffset->m_flWeight);
	}
	bool sequence_change_networked() {
		return *(bool*)(std::uintptr_t(this) + g_pSchemaOffset->m_bSequenceChangeNetworked);
	}
	bool discontinuity() {
		return *(bool*)(std::uintptr_t(this) + g_pSchemaOffset->m_bDiscontinuity);
	}
	float prev_cycle_from_discontinuity() {
		return *(float*)(std::uintptr_t(this) + g_pSchemaOffset->m_flPrevCycleFromDiscontinuity);
	}
	float prev_cycle_for_anim_event_detection() {
		return *(float*)(std::uintptr_t(this) + g_pSchemaOffset->m_flPrevCycleForAnimEventDetection);
	}
};

class c_anim_graph_networked_variables
{

};

class c_base_anim_graph_controller
{
public:
	__forceinline c_networked_sequence_operation base_layer() {
		return *reinterpret_cast<c_networked_sequence_operation*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_baseLayer);
	};
	__forceinline bool sequence_finished() {
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_bSequenceFinished);
	};
};