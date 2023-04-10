#pragma once

#include "../../virtual.hpp"

class CLocalize {
   public:
    auto FindSafe(const char* tokenName) {
        return CALL_VIRTUAL(const char*, 17, this, tokenName);
    }
};
