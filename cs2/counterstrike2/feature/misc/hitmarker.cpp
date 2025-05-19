#include "hitmarker.h"
#include "../utilities/cloud/user.h"

//void c_hitmarker::add(vector position) {
//	mutex.lock();
//	logs.push_back(hitmarker_log(position, Interfaces::globals->global_storage.curtime + 4.f));
//	mutex.unlock();
//}

#include <chrono> // Include this for time handling

void c_hitmarker::add(vector position) {
    auto now = std::chrono::steady_clock::now();
    mutex.lock();
    logs.push_back(hitmarker_log(position, now));
    mutex.unlock();
}


void c_hitmarker::update_world_screen() {
	mutex.lock();

	for (int i = 0; i < logs.size(); i++) {
		logs[i].is_world_screen = math::world_to_screen(logs[i].position, logs[i].world_position);
	}

	mutex.unlock();
}

void c_hitmarker::run()
{
    if (!g_user->IsActive(xorstr_("misc_hitmarker"), xorstr_("misc_hitmarker"), 0))
        return;

    auto now = std::chrono::steady_clock::now();

    mutex.lock();

    for (auto& log : logs) {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - log.time).count();
        const int lifespan_seconds = 4; // Adjust as needed

        if (duration >= lifespan_seconds) {
            log.time = std::chrono::steady_clock::time_point(); // Set to default time point to indicate expiration
        }

        if (!log.is_world_screen)
            continue;

        log.animation.run(duration < lifespan_seconds);

        constexpr float gap = 2.f;
        constexpr float size = 4.f;

        auto color = render_color::white().override_alpha(log.animation.base);

        g_render->line(render_position(log.world_position.x - gap, log.world_position.y - gap), render_position(log.world_position.x - gap - size, log.world_position.y - gap - size), color);
        g_render->line(render_position(log.world_position.x - gap, log.world_position.y + gap), render_position(log.world_position.x - gap - size, log.world_position.y + gap + size), color);
        g_render->line(render_position(log.world_position.x + gap, log.world_position.y - gap), render_position(log.world_position.x + gap + size, log.world_position.y - gap - size), color);
        g_render->line(render_position(log.world_position.x + gap, log.world_position.y + gap), render_position(log.world_position.x + gap + size, log.world_position.y + gap + size), color);
    }

    if (!logs.empty())
    {
        logs.erase(std::remove_if(logs.begin(), logs.end(), [](const auto& x) {
            return x.time == std::chrono::steady_clock::time_point() && x.animation.base <= 0.f;
            }), logs.end());
    }

    mutex.unlock();
}


//void c_hitmarker::run()
//{
//	if (!g_user->IsActive(xorstr_("misc_hitmarker"), xorstr_("misc_hitmarker"), 0))
//		return;
//
//	mutex.lock();
//
//	for (int i = 0; i < logs.size(); i++) {
//
//		if (Interfaces::m_network_game_service && Interfaces::m_network_game_service->get_network_game_client() && Interfaces::m_network_game_service->get_network_game_client()->global_vars.global_storage.curtime >= logs[i].time)
//			logs[i].time = 0.f;
//
//		if (!logs[i].is_world_screen)
//			continue;
//
//		logs[i].animation.run(logs[i].time > 0.f);
//
//		constexpr float gap = 2.f;
//		constexpr float size = 4.f;
//
//		auto color = render_color::white().override_alpha(logs[i].animation.base);
//
//		g_render->line(render_position(logs[i].world_position.x - gap, logs[i].world_position.y - gap), render_position(logs[i].world_position.x - gap - size, logs[i].world_position.y - gap - size), color);
//		g_render->line(render_position(logs[i].world_position.x - gap, logs[i].world_position.y + gap), render_position(logs[i].world_position.x - gap - size, logs[i].world_position.y + gap + size), color);
//		g_render->line(render_position(logs[i].world_position.x + gap, logs[i].world_position.y - gap), render_position(logs[i].world_position.x + gap + size, logs[i].world_position.y - gap - size), color);
//		g_render->line(render_position(logs[i].world_position.x + gap, logs[i].world_position.y + gap), render_position(logs[i].world_position.x + gap + size, logs[i].world_position.y + gap + size), color);
//	}
//
//	if (!logs.empty())
//	{
//		logs.erase(std::remove_if(logs.begin(), logs.end(), [](const auto& x) {
//			return x.time <= 0.f && x.animation.base <= 0.f;
//			}), logs.end());
//	}
//
//	mutex.unlock();
//}