// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"
#include "../../hooks/hooks.hpp"
#include <iostream>
#include <bitset>
#include "../../sdk/interfaces/i_entity_list.hpp"
#include "../../sdk/classes/handle.hpp"

c_weapon_cs_base* c_cs_player_pawn::get_weapon_active()
{
    auto service = weapon_services();

    if (!service)
        return nullptr;

    auto handle = service->m_hactive_weapon();

    if (!handle.is_valid())
        return nullptr;

    auto weapon = reinterpret_cast<c_weapon_cs_base*>(Interfaces::entity_list->get_entity(handle.get_index()));

    if (!weapon)
        return nullptr;

    if (!weapon->find_class(HASH("C_CSWeaponBase")))
        return nullptr;

    return weapon;
}

void c_weapon_cs_base::calc_spread(c_weapon_cs_base_v_data* data, int mode, unsigned int seed, float innacuracy, float spread, float recoil_index, float* spread_x, float* spread_y)
{
    using fn = void(__fastcall*)(void*, void*, int, unsigned int, float, float, float, float*, float*);
    static auto fnspread = reinterpret_cast<fn>(g_utils->pattern_scan(g_cs2->m_module_system.get_client(), "48 8B C4 48 89 58 08 48 89 68 18 48 89 70 20 57 41 54 41 55 41 56 41 57 48 81 EC E0 00 00 00").as<fn*>());
    return fnspread(this, data, mode, seed, innacuracy, spread, recoil_index, spread_x, spread_y);
}

c_cs_player_controller* c_cs_player_pawn::get_controller()
{
    if (!handle_controller().is_valid())
        return nullptr;

    int index = handle_controller().get_index();
    return Interfaces::entity_list->get_controller(index);
}

void c_cs_player_pawn::create_shocked_effect()
{
    using function_t = void(__fastcall*)(const char* name, c_effect_data* data);

    c_effect_data effect = { };
    effect.vftable = g_cs2->m_patterns[FNV32("effect_data_vtable")].as<void*>();
    effect.set_entity(g_cs2->get_handle_entity(this));
    effect.set_other_entity(static_cast<uint32_t>(-1));

    g_cs2->m_patterns[FNV32("dispatch_effect")].as<function_t>()("csshocked", &effect);

    g_cs2->killed_target = nullptr;
    g_cs2->target_killed = false;
}

void c_cs_player_pawn::get_bone_position(std::uint32_t bone_index, vector& position, vector& rotation)
{
    using function_t = std::int64_t(__fastcall*)(c_cs_player_pawn*, std::uint32_t, vector*, vector*);
    g_cs2->m_patterns[FNV32("get_bone_position")].as<function_t>()(this, bone_index, &position, &rotation);
}

vector_2d c_cs_player_pawn::world_space_center()
{
    auto sign = g_utils->get_virtual(this, 72);
    vector out;
    auto ret = reinterpret_cast<vector(__thiscall*)(c_cs_player_pawn*, vector*)>(sign)(this, &out);
    return {};
}

bool c_cs_player_pawn::is_enemy(c_cs_player_pawn* local)
{
    auto local_player = local;

    if (!local)
        local_player = g_cs2->local_player;

    if (!local_player)
        return false;

    if (this == local_player)
        return false;

    if (g_cs2->mp_teammates_are_enemies->GetValue<bool>())
        return true;

    if (team_num() == local_player->team_num())
        return false;

    return true;
}

bool c_cs_player_pawn::has_c4()
{
    return this->weapon_services() && this->weapon_services()->weapon_owns_this_type("weapon_c4");
}

vector c_cs_player_pawn::get_eye_position()
{
    return this->game_scene_node()->abs_origin() + this->view_offset();
}

bool c_cs_player_pawn::is_smoke_view()
{
    if (!collision_property())
        return false;

    //48 83 EC 28 45 33 C0 E8

    auto obb_min = collision_property()->mins();
    auto obb_max = collision_property()->maxs();
    auto obb_center = (obb_max + obb_min) / 2;
    auto obb_position = obb_center + game_scene_node()->abs_origin();

    auto eye_position = get_eye_angle();

    return g_cs2->m_patterns[FNV32("smoke_view")].as<bool(__fastcall*)(vector&, vector&)>()(eye_position, obb_position);
}

bool c_cs_player_pawn::is_attachment()
{
    if (game_scene_node()->parent())
    {
        auto owner = game_scene_node()->parent()->owner();
        if (game_scene_node()->parent()->owner() && game_scene_node()->parent()->owner() == g_cs2->local_player)
        {
            return true;
        }
    }

    return false;
}

int c_cs_player_pawn::get_bone_index(const char* name)
{
    using function_t = int(__fastcall*)(void*, const char*);
    return g_cs2->m_patterns[FNV32("get_bone_index")].as<function_t>()(this, name);
}

int c_cs_player_pawn::get_lerp_tick(int player_tick_count)
{
    unsigned int tick = -1, tick2 = -1, tick3 = -1;
    g_cs2->m_patterns[FNV32("get_lerp_tick")].as<void(__fastcall*)(c_game_scene_node*, unsigned int*, unsigned int*, unsigned int*, int*)>()(game_scene_node(), &tick, &tick2, &tick3, &player_tick_count);
    return tick;
}

static CachedEntity_t::Type GetEntityType(c_base_entity* pEntity);

static CachedEntity_t::Type GetEntityType(c_base_entity* pEntity) {
    if (!pEntity)
        return CachedEntity_t::UNKNOWN;

    if (pEntity->is_controller()) {
        return CachedEntity_t::PLAYER_CONTROLLER;
    }

    return CachedEntity_t::UNKNOWN;
}

std::unordered_multimap<CachedEntity_t::Type, entity_callbacks> g_Callbacks;
std::vector<CachedEntity_t> g_cachedEntities;
std::mutex g_cachedEntitiesMutex;

namespace EntCache {

    void CacheCurrentEntities() {
        // This function runs ONCE on injection and caches all the entities if
        // you happen to inject connected on a server.

        if (!Interfaces::engine->is_connected() || !Interfaces::engine->is_in_game())
            return;

        if (!Interfaces::entity_list) return;

        int highestIndex = Interfaces::entity_list->get_highest_entity_index();
        for (int i = 1; i <= highestIndex; ++i) {
            c_base_entity* pEntity = Interfaces::entity_list->get_entity(i); // Correct method name
            if (!pEntity) continue;

            OnAddEntity(pEntity, pEntity->get_ref_handle());
        }
    }

    void OnAddEntity(c_entity_instance* inst, c_base_handle handle) {
        if (!Interfaces::entity_list) return;

        c_base_entity* pEntity = (c_base_entity*)inst;
        if (!pEntity) return;

        // Cache only networked entities.
        // https://developer.valvesoftware.com/wiki/Entity_limit#Source_2_limits
        if (handle.GetEntryIndex() >= 16384) return;

        if (g_Callbacks.empty()) {
            return;
        }

        // call callbacks
        for (auto& [callback_type, callbacks] : g_Callbacks) {
            if (!callbacks.add || callback_type != GetEntityType(pEntity))
                continue;
            callbacks.add(handle.GetEntryIndex(), handle);
        }
        std::lock_guard<std::mutex> lock(g_cachedEntitiesMutex);

        auto it = std::find_if(g_cachedEntities.begin(), g_cachedEntities.end(),
            [handle]( CachedEntity_t& i) {
                return i.m_handle.GetEntryIndex() == handle.GetEntryIndex();
            });

        if (it == g_cachedEntities.end()) {
            CachedEntity_t cachedEntity{};
            cachedEntity.m_handle = handle;
            cachedEntity.m_type = GetEntityType(pEntity);
            cachedEntity.index = handle.GetEntryIndex();
            cachedEntity.reset = false;

            if (cachedEntity.m_type != CachedEntity_t::UNKNOWN) {
                g_cachedEntities.emplace_back(cachedEntity);
            }
        }
        else {
            it->m_handle = handle;
            it->m_type = GetEntityType(pEntity);
        }
    }

    void OnRemoveEntity(c_entity_instance* inst, c_base_handle handle) {
        if (!Interfaces::entity_list) return;

        c_base_entity* pEntity = (c_base_entity*)inst;
        if (!pEntity) return;

        if (g_Callbacks.empty()) {
            return;
        }

        // call callbacks
        for (auto& [callback_type, callbacks] : g_Callbacks) {
            if (!callbacks.remove)
                continue;

            if (callback_type != GetEntityType(pEntity))
                continue;

            callbacks.remove(handle.GetEntryIndex(), handle);
        }

        std::lock_guard<std::mutex> lock(g_cachedEntitiesMutex);

        auto it = std::find_if(g_cachedEntities.begin(), g_cachedEntities.end(),
            [handle](const CachedEntity_t& i) {
                return i.m_handle == handle;
            });

        if (it == g_cachedEntities.end()) return;

        it->m_draw = false;
        it->m_type = CachedEntity_t::UNKNOWN;

    }

    void RegisterCallback(CachedEntity_t::Type type, entity_callback add, entity_callback remove) noexcept {
        if (!add && !remove)
            return;

       // std::unique_lock lock(g_cachedEntitiesMutex);
        g_Callbacks.insert(std::make_pair(type, entity_callbacks{ add, remove }));

        if (!add)
            return;

        // add existing entities
        for (auto& cached_entity : g_cachedEntities) {
            if (cached_entity.m_type == type)
                add(cached_entity.m_handle.GetEntryIndex(), cached_entity.m_handle);
        }
    }
}
