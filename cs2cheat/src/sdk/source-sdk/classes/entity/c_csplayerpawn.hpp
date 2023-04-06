#pragma once

#include "c_csplayerpawnbase.hpp"

class C_CSPlayerPawn : public C_CSPlayerPawnBase {
   public:
    bool IsEnemyWithTeam(int team);
};
