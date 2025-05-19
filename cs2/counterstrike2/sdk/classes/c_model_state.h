#pragma once

class c_model_state
{
public:
    __forceinline strong_handle<c_model>& m_hModel() {
        return *reinterpret_cast<strong_handle<c_model>*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_hModel);
    };
    __forceinline unsigned long long MeshGroupMask() {
        return *reinterpret_cast<unsigned long long*>(reinterpret_cast<std::uint64_t>(this) + g_pSchemaOffset->m_MeshGroupMask);
    };

    c_bone_data*& get_bone_data() {
        return *(c_bone_data**)(std::uintptr_t(this) + 0x80);
    }

    std::uint8_t padding_0[0x80];
    c_bone_data* bones;
};