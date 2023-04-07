#pragma once

#include "../../../virtual.hpp"

using CSharedObject = void;

class CGCClientSharedObjectTypeCache {
   public:
    auto AddObject(CSharedObject* pObject) {
        return CALL_VIRTUAL(bool, 2, this, pObject);
    }
};
