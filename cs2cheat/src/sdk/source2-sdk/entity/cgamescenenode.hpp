#pragma once

#include "../../math/types/vector.hpp"
#include "../../schema/schema.hpp"

class CGameSceneNode {
   public:
    void SetMeshGroupMask(uint64_t meshGroupMask);

    SCHEMA_FIELD(m_vecAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin", Vector);
};
