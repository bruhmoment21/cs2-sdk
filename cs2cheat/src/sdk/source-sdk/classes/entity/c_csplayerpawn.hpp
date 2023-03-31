#pragma once

#include "c_baseplayerpawn.hpp"

class C_CSPlayerPawn : public C_BasePlayerPawn {
   public:
    bool IsEnemyToLocalPlayer(int lpTeam);
};
