#pragma once

#include "../gcsdk/csharedobject.hpp"

class CEconDefaultEquippedDefinitionInstanceClient : public CSharedObject {
   public:
    auto& GetDefinitionIndex() {
        return *reinterpret_cast<short*>((uintptr_t)(this) + 0x24);
    }
};
