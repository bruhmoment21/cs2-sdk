#pragma once

#include "../../virtual.hpp"

class CSharedObject {
   public:
    auto GetTypeID() { return CALL_VIRTUAL(int, 1, this); }
};
