// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../sdk.hpp"

uintptr_t C_KeyString::FindKey(const char* name) {
    using type = uintptr_t(__fastcall*)(const char*, int, int);
    type fn = g_cs2->m_patterns[FNV32("FindVarMaterial")].as<type>(); // particles.dll E8 ? ? ? ? 80 7F 1A 00 + 1
    return fn(name, 0x12, 0x31415926);
}

void c_resource_system::enumerate_material(std::uint64_t hash, resource_array_t* array, std::uint8_t flags)
{
    using function_t = void(__fastcall*)(c_resource_system*, std::uint64_t, resource_array_t*, std::uint8_t);
    g_cs2->m_patterns[FNV32("enumerate_material")].as<function_t>()(this, hash, array, flags);
}

void i_material_system::SetMaterialShaderType(void* data, const char* shader_name)
{
    using type = void(__fastcall*)(void*, C_KeyString, const char*, int);

    type fn = g_cs2->m_patterns[FNV32("SetMaterialShaderType")].as<type>(); // particles.dll  E8 ? ? ? ? 48 8D B7 ? ? ? ? + 1

    C_KeyString shader((const char*)xorstr_("shader"), 0x162C1777);
    fn(data, shader, shader_name, 0x18);
}

void i_material_system::SetMaterialFunctions(void* data, const char* functions_name, int value)
{
    using type = void(__fastcall*)(void*, C_KeyString, int, int);
    type set_other = g_cs2->m_patterns[FNV32("SetMaterialFunctions")].as<type>(); //particles.dll  E8 ? ? ? ? 49 C1 E5 21 + 1

    C_KeyString functions(functions_name, true);

    set_other(data, functions, value, 0x13);
}

material_paramater* i_material::find_parameter(const char* name)
{
    using type = material_paramater * (__fastcall*)(i_material*, const char*);

    type fn = g_cs2->m_patterns[FNV32("find_param")].as<type>();

    return fn(this, name);
}

void i_material::update_parameter()
{
    using type = void(__fastcall*)(i_material*);

    type fn = g_cs2->m_patterns[FNV32("update_param")].as<type>();

    fn(this);
}

void i_material::ColorModulate(const color_t& colNewColor) {
    material_paramater* pColorParameter = find_parameter("g_vColorTint");
    if (!pColorParameter)
        return;

    vector_4d colorVector(colNewColor.get_red() / 255.0f, colNewColor.get_green() / 255.0f, colNewColor.get_blue() / 255.0f, colNewColor.get_alpha() / 255.0f);
    pColorParameter->vec_value = colorVector;

    update_parameter();
}

