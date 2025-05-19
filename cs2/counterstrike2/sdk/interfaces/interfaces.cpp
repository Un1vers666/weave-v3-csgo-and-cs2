// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "interfaces.hpp"

#include "../../signatures.hpp"
#include "../../utilities/utilities.hpp"
#include "../../utilities/debug_console/debug.hpp"
#include "../../utilities/hash.h"

template<typename T>
T* Interfaces::get_interface(std::uint64_t module, const char* interface_name, bool hash, std::uint64_t hash_interface)
{
	if (!interface_name)
		return nullptr;

	if(!module)
		return nullptr;

	auto create_interface = g_utils->get_proc_address(module, xorstr_("CreateInterface"));
	if (!create_interface)
		return nullptr;


	using interface_callback_fn = void* (__cdecl*)();

	typedef struct _interface_reg_t
	{
		interface_callback_fn callback;
		const char* name;
		_interface_reg_t* flink;
	} interface_reg_t;

	interface_reg_t* interface_list = *reinterpret_cast<interface_reg_t**>(g_utils->resolve_rip(create_interface, 3));

	if (!interface_list)
		return nullptr;


	for (interface_reg_t* it = interface_list; it; it = it->flink)
	{
		if (hash)
		{
			if (HASH_RT(it->name) == hash_interface)
				return reinterpret_cast<T*>(it->callback);
		}
		else {
			if (!strcmp(it->name, interface_name))
				return reinterpret_cast<T*>(it->callback());
		}

	}

	return nullptr;
}

#include <fstream>
#include "../../Logging.h"
#include "../../loader/loader.h"

bool Interfaces::initialize()
{
	add_trace(__PRETTY_FUNCTION__);
	auto p = get_interface<std::uintptr_t>(g_cs2->m_module_system.get_scenesystem(), xorstr_("SceneUtils_001"));
	m_scene_utils = get_interface<c_scene_utils>(g_cs2->m_module_system.get_scenesystem(), xorstr_("SceneUtils_001"));
	m_application = g_cs2->m_patterns[FNV32("application")].cast<c_application*>();
	m_command_line = reinterpret_cast<c_command_line * (__fastcall*)()>(g_utils->get_proc_address(g_cs2->m_module_system.get_tier0(), xorstr_("CommandLine")))();
	m_mem_alloc = *(c_memalloc**)g_utils->get_proc_address(g_cs2->m_module_system.get_tier0(), "g_pMemAlloc");

	m_game_particle_manager_system = g_cs2->m_patterns[FNV32("game_particle_manager")].as<c_game_particle_manager_system * (__cdecl*)()>()();

	csgo_input = g_cs2->m_patterns[FNV32("csgo_input")].cast<i_csgo_input*>();
	pvs_manager = g_utils->pattern_scan(g_cs2->m_module_system.get_engine2(), "48 8D 0D ? ? ? ? 33 D2 FF 50").get_absolute_address(3, 0).as<c_pvs_manager*>();
	entity_list = g_cs2->m_patterns[FNV32("game_entity_system")].cast<i_entity_list*>();
	trace = g_cs2->m_patterns[FNV32("trace")].cast<i_trace*>();
	event_manager = g_cs2->m_patterns[FNV32("game_event_manager")].cast<game_event_manager*>();
	setup_interpolation_fractions = reinterpret_cast<fnSetupInterpolationFractions>(g_utils->pattern_scan(g_cs2->m_module_system.get_client(), "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC 70 48 8B F9 48 8D 44 24").as<fnSetupInterpolationFractions*>());
	get_button_state = reinterpret_cast<fnGetButtonState>(g_utils->pattern_scan(g_cs2->m_module_system.get_client(), "4C 63 C2 4B 8D 04").as<fnGetButtonState*>());
	m_client_mode = g_cs2->m_patterns[FNV32("client_mode")].cast<c_client_mode*>();
	m_render_game_system = g_cs2->m_patterns[FNV32("render_game_system")].cast<c_render_game_system*>(); // xref: pointer -> RenderGameSystem -> CViewSetup::OnRenderStart -> C:\\buildworker\\csgo_rel_win64\\build\\src\\game\\client\\view.cpp

	if (m_network_game_service)
		m_network_game_client = m_network_game_service->get_network_game_client();

	if (m_network_game_client)
		globals = g_cs2->m_patterns[FNV32("global_vars")].cast<c_global_vars*>();

;	m_game_rules = g_cs2->m_patterns[FNV32("game_rules")].cast<c_cs_game_rules*>();
	m_fx_queue_manager = g_cs2->m_patterns[FNV32("fx_queue_manager")].cast<c_fx_queue_manager_impl*>();
	m_scene_view_debug_overlays = g_cs2->m_patterns[FNV32("get_scene_view_debug_overlay")].cast<c_scene_view_debug_overlays*>();
	m_particle_manager =  g_cs2->m_patterns[FNV32("particle_manager")].cast<c_particle_manager*>();

	auto inventory_manager = g_cs2->m_patterns[FNV32("inventory_manager")].get();
	m_cs_inventory_manager = *reinterpret_cast<c_cs_inventory_manager**>(&inventory_manager);

	auto gc_client = g_cs2->m_patterns[FNV32("gc_client")].get();
	m_gc_client = *reinterpret_cast<c_gc_client_system**>(&gc_client);
	econ_item_system = *(c_econ_item_system**)(std::uintptr_t(g_cs2->m_patterns[FNV32("econ_item_system")].cast<std::uintptr_t*>()) + 0x8);

	using fn_steam_client = ISteamClient*(__cdecl*)(void);
	using tt = uint64_t(__cdecl*)(void);

	std::uint64_t steam_user = reinterpret_cast<tt>(g_utils->get_proc_address(g_cs2->m_module_system.get_steam_api64(), xorstr_("SteamAPI_GetHSteamUser")))();
	std::uint64_t steam_pipe = reinterpret_cast<tt>(g_utils->get_proc_address(g_cs2->m_module_system.get_steam_api64(), xorstr_("SteamAPI_GetHSteamPipe")))();
	m_steam_client = reinterpret_cast<fn_steam_client>(g_utils->get_proc_address(g_cs2->m_module_system.get_steam_api64(), xorstr_("SteamClient")))();
	m_steam_game_coordinator = reinterpret_cast<ISteamGameCoordinator*>(m_steam_client->GetISteamGenericInterface(steam_user, steam_pipe, xorstr_("SteamGameCoordinator001")));
	m_steam_user = m_steam_client->GetISteamUser(steam_user, steam_pipe, xorstr_("SteamUser023"));
	m_steam_friends = m_steam_client->GetISteamFriends(steam_user, steam_pipe, xorstr_("SteamFriends017"));
	m_steam_utils = m_steam_client->GetISteamUtils(steam_pipe, xorstr_("SteamUtils010"));

	CSteamID steamID = m_steam_user->GetSteamID();

	const char* steamUsername = m_steam_friends->GetFriendPersonaName(steamID);

	g_loader = std::make_unique<c_loader>();
	strncpy(g_loader->username, steamUsername, sizeof(steamUsername));

	g_loader->Setup();

	//if (!Interfaces::m_command_line->find("-insecure")) {
	//	MessageBoxA(0, "add -insecure to the game parameters in order to play", 0, 0);
	//	exit(0);
	//}

	if (!convars ||
		!csgo_input ||
		!engine ||
		!input_system ||
		!schema_system ||
		!trace ||
		!globals ||
		!material_system ||
		!event_manager ||
		!m_steam_client ||
		!m_steam_game_coordinator ||
		!m_steam_user ||
		!m_steam_friends ||
		!m_steam_utils ||
		!m_client_mode ||
		!m_render_game_system
		|| !pvs_manager)
	{
		LogError("failed to initialize interfaces");
		return false;
	}
	LogInfo("interfaces done");
	return true;
}