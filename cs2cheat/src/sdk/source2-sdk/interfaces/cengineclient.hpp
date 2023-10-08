#pragma once

#include "../../virtual.hpp"

class CEngineClient {
   public:
    auto IsInGame() { return CALL_VIRTUAL(bool, 32, this); }
};
