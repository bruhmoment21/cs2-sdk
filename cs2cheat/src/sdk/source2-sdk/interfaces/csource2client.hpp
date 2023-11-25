#pragma once

#include "../../virtual.hpp"

#include "../econ/ceconitemsystem.hpp"

class CSource2Client {
   public:
    auto GetEconItemSystem() {
        return CALL_VIRTUAL(CEconItemSystem*, 113, this);
    }
};
