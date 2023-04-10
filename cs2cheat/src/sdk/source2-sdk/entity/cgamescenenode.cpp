#include "cgamescenenode.hpp"

#include "../../memory/memory.hpp"

void CGameSceneNode::SetMeshGroupMask(uint64_t meshGroupMask) {
    if (!memory::fnSetMeshGroupMask) return;
    return memory::fnSetMeshGroupMask(this, meshGroupMask);
}
