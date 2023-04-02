#pragma once

#include <cstdint>

#define ENT_ENTRY_MASK 0x7fff

class C_BaseEntity;

class CHandle {
    C_BaseEntity* GetBaseEntity() const;

   public:
    inline int GetEntryIndex() const { return m_Index & ENT_ENTRY_MASK; }
    template <typename T>
    inline T* Get() const {
        return reinterpret_cast<T*>(GetBaseEntity());
    }

    uint32_t m_Index;
};
