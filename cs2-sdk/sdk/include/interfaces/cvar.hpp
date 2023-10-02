#pragma once

#include <pointer/pointer.hpp>

class ConVar {
   public:
    const char* m_Name;

    template <typename T>
    T GetValue() {
        return CPointer(this).GetField<T>(0x40);
    }
};

class CCVar {
   public:
    using CVarIterator_t = uint64_t;

    static CCVar* Get();

    CVarIterator_t GetFirstCvarIterator();
    CVarIterator_t GetNextCvarIterator(CVarIterator_t prev);

    ConVar* GetCvarByIndex(CVarIterator_t index);
    ConVar* GetCvarByName(const char* name);
};
