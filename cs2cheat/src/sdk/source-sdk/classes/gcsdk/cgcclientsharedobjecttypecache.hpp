#pragma once

#include "../../../virtual.hpp"

#include "../types/utlvector.hpp"

using CSharedObject = void;

class CGCClientSharedObjectTypeCache {
   public:
    auto AddObject(CSharedObject* pObject) {
        return CALL_VIRTUAL(bool, 2, this, pObject);
    }

    template <typename T>
    auto& GetVecObjects() {
        return *reinterpret_cast<CUtlVector<T>*>((uintptr_t)(this) + 0x8);
    }
};
