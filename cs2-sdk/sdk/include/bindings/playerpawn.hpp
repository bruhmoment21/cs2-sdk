#pragma once

#include <bindings/baseentity.hpp>

class C_CSPlayerPawnBase : public C_BaseEntity {
   public:
    bool IsPlayerPawn();
    bool IsObserverPawn();
};
