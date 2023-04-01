#pragma once

#include "memory/memory.hpp"

#define CALL_VIRTUAL(retType, idx, ...) \
    vmt::CallVirtual<retType>(idx, __VA_ARGS__)

namespace vmt {
    template <typename T, typename... Args>
    inline T CallVirtual(uint32_t uIndex, void* pClass, Args... args) {
        CS2_ASSERT(pClass);

        using FnType = T(__thiscall***)(void*, Args...);
        auto pFunction = (*static_cast<FnType>(pClass))[uIndex];

        return pFunction(pClass, args...);
    }

    template <typename T = void*>
    inline T GetVMethod(uint32_t uIndex, void* pClass) {
        CS2_ASSERT(pClass);

        void** pVTable = *static_cast<void***>(pClass);
        return reinterpret_cast<T>(pVTable[uIndex]);
    }
}  // namespace vmt
