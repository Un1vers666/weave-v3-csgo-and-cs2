// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "c_econ_item.hpp"
#include <tinyformat/tinyformat.h>

void c_econ_item::set_dynamic_attribute_value(int index, void* value) {
    auto pItemSchema = Interfaces::econ_item_system;
    if (!pItemSchema) 
        return;

    void* attribute_definition_interface = pItemSchema->get_attribute_definition_interface(index);
    if (!attribute_definition_interface)
        return;

    using function_t = void(__fastcall*)(c_econ_item*, void*, void*);

    g_cs2->m_patterns[FNV32("set_dynamic_attribute_value")].as<function_t>()(this, attribute_definition_interface, value);
}

void c_econ_item::set_dynamic_attribute_value_string(int index, const char* value) {
    //48 89 7C 24 18 4C 89 64 24 20 55 41 56 41 57 48 8D 6C 24 B9 48 81 EC B0
    auto pItemSchema = Interfaces::econ_item_system;
    if (!pItemSchema)
        return;

    void* attribute_definition_interface = pItemSchema->get_attribute_definition_interface(index);
    if (!attribute_definition_interface)
        return;

    using function_t = void(__fastcall*)(c_econ_item*, void*, const char**);

    g_cs2->m_patterns[FNV32("set_dynamic_attribute_value_string")].as<function_t>()(this, attribute_definition_interface, &value);
}

void c_econ_item::set_custom_name(const char* name)
{
    auto index = Interfaces::econ_item_system->get_attribute_definition_by_name(xorstr_("custom name attr"));
    auto schema = index->m_pSchemaAttributeType_String;

    std::uintptr_t* penis = (std::uintptr_t*)malloc(0x18);
    ZeroMemory(penis, 0x18);

    CALL_VIRTUAL(bool, 4, schema, index, name, penis);

    g_cs2->m_patterns[FNV32("set_dynamic_attribute_value_string")].as<void(__fastcall*)(c_econ_item*, void*, CAttributeString*)>()(this, index, *(CAttributeString**)(penis));
}

void c_econ_item::set_sticker(int index, int id)
{
    auto econ_item_attribute_definition = Interfaces::econ_item_system->get_attribute_definition_by_name(tfm::format(xorstr_("sticker slot %d id"), index).c_str());

    if (!econ_item_attribute_definition)
        return;

    using function_t = void(__fastcall*)(c_econ_item*, CEconItemAttributeDefinition*, void*);

    g_cs2->m_patterns[FNV32("set_dynamic_attribute_value")].as<function_t>()(this, econ_item_attribute_definition, &id);
}

c_econ_item* c_econ_item::create_instance() {

    using function_t = c_econ_item*(__fastcall*)(void);
    return g_cs2->m_patterns[FNV32("econ_item_initilization")].as<function_t>()();
}