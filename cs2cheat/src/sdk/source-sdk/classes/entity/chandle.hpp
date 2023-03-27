#pragma once

#include <cstdint>

#define ENT_ENTRY_MASK 0x7fff

class C_BaseEntity;

class CHandle {
   public:
    C_BaseEntity* Get() const;

    inline int GetEntryIndex() const { return m_Index & ENT_ENTRY_MASK; }
    template <typename T>
    inline T* GetAs() const {
        return (T*)(Get());
    }

    uintptr_t m_Index;
};
