#include "../../sdk/sdk.hpp"
#include "sky_box.h"
#include "../../Logging.h"
#include "../utilities/cloud/user.h"
#include "../chams/chams.h"

i_material* c_skybox::reconstruct_sky_material(i_material* mat) {
    std::cout << "reconstructing material: " << mat->get_name() << std::endl;

    std::string _template = R"#(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "sky.vfx"
                F_TEXTURE_FORMAT2 = 6
                g_flBrightnessExposureBias = -0.5
                g_flRenderOnlyExposureBias = 0
                SkyTexture = resource:"materials/skybox/tests/src/lightingtest_sky_night.png"
                g_tSkyTexture = resource:"materials/skybox/tests/src/lightingtest_sky_night_exr_2c5e8c62.vtex"
			} )#";

    i_material* material = g_chams->create_material(_template.c_str());
    if (!material) {
        return nullptr;
    }

    if (mat->find_parameter("F_TEXTURE_FORMAT2"))     material->find_parameter("F_TEXTURE_FORMAT2")->texture_value = mat->find_parameter("F_TEXTURE_FORMAT2")->texture_value;
    if (mat->find_parameter("g_flBrightnessExposureBias"))     material->find_parameter("g_flBrightnessExposureBias")->texture_value = mat->find_parameter("g_flBrightnessExposureBias")->texture_value;
    if (mat->find_parameter("g_flRenderOnlyExposureBias"))    material->find_parameter("g_flRenderOnlyExposureBias")->texture_value = mat->find_parameter("g_flRenderOnlyExposureBias")->texture_value;
    if (mat->find_parameter("SkyTexture"))    material->find_parameter("SkyTexture")->texture_value = mat->find_parameter("SkyTexture")->texture_value;
    if (mat->find_parameter("g_tSkyTexture"))    material->find_parameter("g_tSkyTexture")->texture_value = mat->find_parameter("g_tSkyTexture")->texture_value;

    return material;
}

void c_skybox::sky_box_changer() {
    static uint64_t oldArrCount = 0;
    resource_array_t arr{};

    arr.m_nCount = 0;
    arr.m_aResources = nullptr;
    Interfaces::resource_system->enumerate_material(0x74616D76, &arr, 2);

    if (!arr.m_nCount)
        return;

    oldArrCount = arr.m_nCount;

    for (int i = 0; i < arr.m_nCount; ++i) {
        i_material** pmat = arr.m_aResources[i];

        if (!pmat)
            continue;

        i_material* mat = *pmat;
        if (!mat)
            continue;

        std::string name = mat->get_name();

        if (name.find("sky") != std::string::npos) {

            i_material* material = reconstruct_sky_material(mat);
            *pmat = material;
            mat = material;
            mat->update_parameter();
        }
    }
}

void c_skybox::fsn(int stage) {
    //if (stage == 6) {
    //    sky_box_changer();
    //}
}