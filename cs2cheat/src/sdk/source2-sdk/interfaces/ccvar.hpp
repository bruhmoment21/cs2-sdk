#pragma once

#include "../../virtual.hpp"

class ConVar {
   public:
    const char* m_name;

    template <typename T>
    T GetValue() {
        // Tip: Do not modify the value by making this a reference.
        // It'll get your account flagged.
        return *reinterpret_cast<T*>((uintptr_t)(this) + 0x40);
    }
};

class CCvar {
   public:
    auto GetFirstCvarIterator(uint64_t& idx) {
        return CALL_VIRTUAL(void*, 12, this, &idx);
    }

    auto GetNextCvarIterator(uint64_t& idx) {
        return CALL_VIRTUAL(void*, 13, this, &idx, idx);
    }

    auto FindVarByIndex(uint64_t index) {
        return CALL_VIRTUAL(ConVar*, 37, this, index);
    }

    auto FindVarByName(const char* var_name) -> ConVar* {
        // Tip: There's logging in this function because this should run ONLY
        // once for every ConVar. If the console is spammed it means you haven't
        // made the variable static.

        uint64_t i = 0;
        GetFirstCvarIterator(i);
        while (i != 0xFFFFFFFF) {
            ConVar* pCvar = FindVarByIndex(i);
            if (strcmp(pCvar->m_name, var_name) == 0) {
                LOG("CCvar::FindVarByName() found '%s' at -> %p\n", var_name,
                    pCvar);
                return pCvar;
            }
            GetNextCvarIterator(i);
        }

        LOG("CCvar::FindVarByName() couldn't find '%s'.\n", var_name);
        return nullptr;
    }
};
