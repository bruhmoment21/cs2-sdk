#pragma once

#include "cgcclient.hpp"

class CGCClientSystem {
   public:
    static CGCClientSystem* GetInstance();

    CGCClient* GetCGCClient() {
        return reinterpret_cast<CGCClient*>((uintptr_t)(this) + 0xB8);
    }
};
