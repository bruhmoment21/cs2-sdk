#pragma once

#include "../../../virtual.hpp"

class CEconItemDefinition {
   public:
    auto GetDefinitionIndex() { return CALL_VIRTUAL(uint16_t, 0, this); }

    auto GetItemBaseName() {
        return *reinterpret_cast<const char**>((uintptr_t)(this) + 0x70);
    }
};
