#include "../../sdk/sdk.hpp"
#include "world_color.h"
#include "../../Logging.h"
#include "../utilities/cloud/user.h"
#include "../chams/chams.h"

i_material* c_world::reconstruct_material(i_material* mat) {
    //L_PRINT(LOG_INFO) << "reconstructing material: " << mat->GetName();
    if (mat->find_parameter("g_tColorA")) {
        std::string _template = R"#(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "csgo_simple_2way_blend.vfx"
                g_tColorA = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tColorB = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tNormalA = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tNormalB = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                F_IGNOREZ = 0
                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 0
                F_RENDER_BACKFACES = 1
                g_vColorTint = [1.0, 1.0, 1.0, 1.0]
			} )#";

        i_material* material = g_chams->create_material(_template.c_str());
        if (!material) {
            //L_PRINT(LOG_ERROR) << "Failed to reconstruct 2way blend material " << mat->GetName();
            return nullptr;
        }

        if (mat->find_parameter("g_tColorA"))     material->find_parameter("g_tColorA")->texture_value = mat->find_parameter("g_tColorA")->texture_value;
        if (mat->find_parameter("g_tColorB"))     material->find_parameter("g_tColorB")->texture_value = mat->find_parameter("g_tColorB")->texture_value;
        if (mat->find_parameter("g_tNormalA"))    material->find_parameter("g_tNormalA")->texture_value = mat->find_parameter("g_tNormalA")->texture_value;
        if (mat->find_parameter("g_tNormalB"))    material->find_parameter("g_tNormalB")->texture_value = mat->find_parameter("g_tNormalB")->texture_value;

        return material;
    }
    else {
        std::string _template = R"#(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "generic.vfx"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                F_IGNOREZ = 0
                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 0
                F_RENDER_BACKFACES = 1
                g_vColorTint = [1.0, 1.0, 1.0, 1.0]
			} )#";

        i_material* material = g_chams->create_material(_template.c_str());
        if (!material) {
            // L_PRINT(LOG_ERROR) << "Failed to recreate material " << mat->GetName();
            return nullptr;
        }

        if (mat->find_parameter("g_tColor"))     material->find_parameter("g_tColor")->texture_value = mat->find_parameter("g_tColor")->texture_value;
        if (mat->find_parameter("g_tNormal"))    material->find_parameter("g_tNormal")->texture_value = mat->find_parameter("g_tNormal")->texture_value;

        return material;
    }
}

void c_world::world_color_changer() {
    static color_t currentColor{ 69, 69, 69 };
    static uint64_t oldArrCount = 0;
    resource_array_t arr{};

    arr.m_nCount = 0;
    arr.m_aResources = nullptr;
    Interfaces::resource_system->enumerate_material(0x74616D76, &arr, 2);

    color_t targetColor{ 0, 0, 0 };
    color_t world_color(g_user->GetConfig()["misc_world_color"]);
    if (!g_user->IsActive(xorstr_("misc_world"), xorstr_("misc_world"), 0))
        targetColor = { 255, 255, 255 };
    else
        targetColor = world_color;

    if (targetColor == currentColor && oldArrCount == arr.m_nCount)
        return;

    if (!arr.m_nCount)
        return;

    oldArrCount = arr.m_nCount;
    currentColor = targetColor;

    for (int i = 0; i < arr.m_nCount; i++) {

        i_material** pmat = arr.m_aResources[i];

        if (!pmat)
            continue;

        i_material* mat = *pmat;
        if (!mat)
            continue;

        std::string name = mat->get_name();

        // std::cout << "Material Name: " << name << std::endl;

        if (name.find("char") != std::string::npos)
            continue;

        if (name.find("weapons") != std::string::npos)
            continue;

        if (name.find("weapon") != std::string::npos)
            continue;

        if (name.find("loadout") != std::string::npos)
            continue;

        if (name.find("gun") != std::string::npos)
            continue;

        if (name.find("inventory") != std::string::npos)
            continue;

        if (name.find("skin") != std::string::npos)
            continue;

        if (name.find("plane") != std::string::npos)
            continue;

        if (name.find("luggage") != std::string::npos)
            continue;

        if (name.find("suitcase") != std::string::npos)
            continue;

        if (name.find("weave") != std::string::npos)
            continue;

        material_paramater* param = mat->find_parameter("g_vColorTint");

        if (!param) continue;

        if (mat->find_parameter("F_NOTINT") || mat->find_parameter("g_tTintMask")) {
            // we need to recreate the material here because a material with tint mask and / or notint is just unaccaptable
            i_material* material = reconstruct_material(mat);
            *pmat = material;
            mat = material;
            param = material->find_parameter("g_vColorTint");
        }

        param->vec_value = vector_4d{ targetColor.get_red() / 255.f, targetColor.get_green() / 255.f, targetColor.get_blue() / 255.f, 1.f };
        mat->update_parameter();
    }
}

void c_world::fsn(int stage) {
    if (stage == 6) {
        world_color_changer();
    }
}