#pragma once

#include "../../virtual.hpp"

#include "../types/utlvector.hpp"

using CSharedObject = void;

class CGCClientSharedObjectTypeCache {
   public:
    auto AddObject(CSharedObject* pObject) {
        return CALL_VIRTUAL(bool, 1, this, pObject);
    }

    auto RemoveObject(CSharedObject* soIndex) {
        return CALL_VIRTUAL(CSharedObject, 3, this, soIndex);
    }

    template <typename T>
    auto& GetVecObjects() {
        return *reinterpret_cast<CUtlVector<T>*>((uintptr_t)(this) + 0x8);
    }
};
