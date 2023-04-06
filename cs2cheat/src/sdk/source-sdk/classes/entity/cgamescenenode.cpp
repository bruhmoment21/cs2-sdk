#include "cgamescenenode.hpp"

#include "../../../memory/memory.hpp"

void CGameSceneNode::SetMeshGroupMask(uint64_t meshGroupMask) {
    return memory::fnSetMeshGroupMask(this, meshGroupMask);
}
