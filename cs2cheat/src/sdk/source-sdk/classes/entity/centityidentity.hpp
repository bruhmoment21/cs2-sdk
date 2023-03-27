#pragma once

#include "../../netvars/netvars.hpp"
#include "../../math/vector.hpp"

#include "chandle.hpp"

class CEntityIdentity {
   public:
    NETVAR(m_designerName, "CEntityIdentity", "m_designerName", const char*);
};
