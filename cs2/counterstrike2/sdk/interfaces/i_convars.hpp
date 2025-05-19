#pragma once
#include "virtual.hpp"
#include <algorithm>

#define LOG(...) printf(__VA_ARGS__)

union CVValue_t
{
    bool i1;
    short i16;
    uint16_t u16;
    int i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    float fl;
    double db;
    const char* sz;
    color_t clr;
    //vector_2d vec2;
    //vector vec3;
    //vector_4d vec4;
    //vector ang;
};

class convar
{
public:
    const char* m_name; // 0x0000
    convar* m_pNext; // 0x0008
    const char pad000[0x10]; // 0x0010
    const char* szDescription; // 0x0020
    uint32_t nType; // 0x28
    uint32_t nRegistered; // 0x2C
    uint32_t nFlags; // 0x30
    const char pad001[0x8]; // 0x34
    // @note: read-only, mofify with caution
    CVValue_t value; // 0x40

    template <typename T>
    T GetValue() {
        // Tip: Do not modify the value by making this a reference.
        // It'll get your account flagged.
        return *reinterpret_cast<T*>((uintptr_t)(this) + 0x40);
    }
};

//class convar {
//public:
//    const char* m_name;
//
//    template <typename T>
//    T GetValue() {
//        // Tip: Do not modify the value by making this a reference.
//        // It'll get your account flagged.
//        return *reinterpret_cast<T*>((uintptr_t)(this) + 0x40);
//    }
//
//    template <typename T>
//    void SetValue(const T& value) {
//        *reinterpret_cast<T*>((uintptr_t)(this) + 0x40) = value;
//    }
//};

class ccvar {
public:
    auto GetFirstCvarIterator(uint64_t& idx) {
        return CALL_VIRTUAL(void*, 12, this, &idx);
    }

    auto GetNextCvarIterator(uint64_t& idx) {
        return CALL_VIRTUAL(void*, 13, this, &idx, idx);
    }

    auto FindVarByIndex(uint64_t index) {
        return CALL_VIRTUAL(convar*, 37, this, index);
    }

    auto FindVarByName(const char* var_name) -> convar* {
        // Tip: There's logging in this function because this should run ONLY
        // once for every ConVar. If the console is spammed it means you haven't
        // made the variable static.

        uint64_t i = 0;
        GetFirstCvarIterator(i);
        while (i != 0xFFFFFFFF) {
            convar* pCvar = FindVarByIndex(i);
            if (strcmp(pCvar->m_name, var_name) == 0) {
                //LOG(xorstr_("CCvar::FindVarByName() found '%s' at -> %p\n"), var_name,
                 //   pCvar);
                return pCvar;
            }
            GetNextCvarIterator(i);
        }

        //LOG(xorstr_("CCvar::FindVarByName() couldn't find '%s'.\n"), var_name);
        return nullptr;
    }
};
