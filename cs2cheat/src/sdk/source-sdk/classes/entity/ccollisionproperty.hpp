#pragma once

#include "../../../math/vector.hpp"

#include "../../netvars/netvars.hpp"

class CCollisionProperty {
   public:
    NETVAR(m_vecMins, "CCollisionProperty", "m_vecMins", Vector);
    NETVAR(m_vecMaxs, "CCollisionProperty", "m_vecMaxs", Vector);
};
