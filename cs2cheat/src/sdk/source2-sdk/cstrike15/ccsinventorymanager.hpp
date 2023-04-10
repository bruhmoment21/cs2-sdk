#pragma once

#include "../../virtual.hpp"

class CCSPlayerInventory;

class CCSInventoryManager {
   public:
    auto GetLocalInventory() {
        return CALL_VIRTUAL(CCSPlayerInventory*, 53, this);
    }
};
