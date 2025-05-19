#include "misc.h"
#include "../../utilities/dx11_helper/dx11_helper.hpp"
#include "../../renderer/render_manager.h"
#include "../../gui/gui.h"

//void c_misc::notify()
//{
//	render_size size_block = render_size(220, 50);
//	for (int i{}; i < m_notify.size(); i++)
//	{
//		m_notify[i].time_to_de -= 0.010f;
//		auto& anim = m_notify[i].animation;
//
//		auto position = render_position(g_render->get_screen_size().x, (g_render->get_screen_size().y - 10 - size_block.y) - ((10 + size_block.y) * i));
//
//		anim.run(m_notify[i].time_to_de > 0.f, 0.2f);
//
//		g_render->push_override_alpha(anim.base);
//
//		position -= render_position((10 + size_block.x) * anim.base, 0);
//
//		g_render->rect_filled(position, size_block, render_color(16, 16, 16), c_render_manager::render_flags_::blur, 3.f, render_rounding_flags_::ROUND_RECT_TOP);
//		g_render->rect_filled(position, size_block, render_color(16, 16, 16, 180), 0, 3.f, render_rounding_flags_::ROUND_RECT_TOP);
//		g_render->add_texture(position + render_position(14, 11), render_size(50, 23), g_gui->m_texture[1]);
//		g_render->add_text(m_notify[i].title, position + render_position(82, 8), render_color::white(), g_render->get_font(1), 13.f);
//
//
//		g_render->pop_override_alpha();
//	}
//	m_notify.erase(std::remove_if(m_notify.begin(), m_notify.end(), [&](const auto x) {
//		return x.time_to_de < 0.f && x.animation.base <= 0.f;
//		}), m_notify.end());
//}

void c_misc::notify()
{
    for (int i = 0; i < m_notify.size(); ++i)
    {
        m_notify[i].time_to_de -= 0.010f;
        auto& anim = m_notify[i].animation;

        // Calculate position for each notification
        auto position = render_position(10, 10 + (i * 60)); // Adjust Y position based on notification count and size

        // Apply animation
        anim.run(m_notify[i].time_to_de > 0.f, 0.2f);

        g_render->push_override_alpha(anim.base);

        // Calculate text width dynamically
        float textWidth = g_render->get_font(1)->calc_text_size(13.f, FLT_MAX, FLT_MAX, m_notify[i].title.c_str()).x;

        // Adjust block size based on text width
        render_size size_block(textWidth + 100, 50);

        // Render background
        g_render->rect_filled(position, size_block, render_color(16, 16, 16), c_render_manager::render_flags_::blur, 3.f, render_rounding_flags_::ROUND_RECT_TOP);
        g_render->rect_filled(position, size_block, render_color(16, 16, 16, 180), 0, 3.f, render_rounding_flags_::ROUND_RECT_TOP);

        // Render texture (adjust coordinates as needed)
        g_render->add_texture(position + render_position(14, 11), render_size(50, 23), g_gui->m_texture[1]);

        // Render title text
        g_render->add_text(m_notify[i].title, position + render_position(82, 8), render_color::white(), g_render->get_font(1), 13.f);

        g_render->pop_override_alpha();
    }

    // Remove notifications that have expired
    m_notify.erase(std::remove_if(m_notify.begin(), m_notify.end(), [&](const auto& notify) {
        return notify.time_to_de < 0.f && notify.animation.base <= 0.f;
        }), m_notify.end());
}

