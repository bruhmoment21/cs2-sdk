#include "pch.hpp"

#include <virtual/virtual.hpp>
#include <logger/logger.hpp>

CPointer vt::GetMethod(CPointer base, uint32_t index) {
    if (!base.IsValid()) {
        CLogger::Log("[virtual] Tried getting virtual method from a null class.");
        return 0;
    }

    CPointer VTable = base.GetField<uintptr_t>(0);
    if (!VTable.IsValid()) {
        CLogger::Log("[virtual] Tried getting virtual method from a null vtable.");
        return 0;
    }

    return VTable.GetField<uintptr_t>(index * sizeof(uintptr_t));
}
