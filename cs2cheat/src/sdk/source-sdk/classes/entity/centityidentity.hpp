#pragma once

#include "../../../math/vector.hpp"

#include "../../netvars/netvars.hpp"

#include "chandle.hpp"

class CEntityIdentity {
   public:
    NETVAR(m_designerName, "CEntityIdentity", "m_designerName", const char*);
};
