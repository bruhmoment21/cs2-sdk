#include "c_baseentity.hpp"

#include "../../interfaces/interfaces.hpp"
#include "../../memory/memory.hpp"

#include <imgui/imgui_internal.h>

bool C_BaseEntity::IsBasePlayerController() {
    return CALL_VIRTUAL(bool, 144, this);
}

bool C_BaseEntity::IsBasePlayerWeapon() {
    return CALL_VIRTUAL(bool, 150, this);
}

bool C_BaseEntity::IsChicken() {
    SchemaClassInfoData_t* pClassInfo = Schema_DynamicBinding();
    if (!pClassInfo) return false;

    const char* className = pClassInfo->GetName();
    if (!className) return false;

    static constexpr auto C_Chicken = hash_32_fnv1a_const("C_Chicken");
    return hash_32_fnv1a_const(className) == C_Chicken;
}

bool C_BaseEntity::IsViewModel() { return CALL_VIRTUAL(bool, 242, this); }

const Vector& C_BaseEntity::GetOrigin() {
    static const Vector null{};

    CGameSceneNode* pGameSceneNode = m_pGameSceneNode();
    if (!pGameSceneNode) return null;

    return pGameSceneNode->m_vecAbsOrigin();
}

bool C_BaseEntity::GetBoundingBox(BBox_t& out, bool computeSurroundingBox) {
    CCollisionProperty* pCollision = m_pCollision();
    if (!pCollision) return false;

    Vector min{}, max{};
    if (computeSurroundingBox) {
        if (!ComputeHitboxSurroundingBox(min, max)) return false;
    } else {
        const Vector& absOrigin = GetOrigin();
        min = pCollision->m_vecMins() + absOrigin;
        max = pCollision->m_vecMaxs() + absOrigin;
    }

    out.x = out.y = std::numeric_limits<float>::max();
    out.w = out.h = -std::numeric_limits<float>::max();

    for (int i = 0; i < 8; ++i) {
        const Vector point{i & 1 ? max.x : min.x, i & 2 ? max.y : min.y,
                           i & 4 ? max.z : min.z};
        ImVec2 screen;
        if (!math::WorldToScreen(point, screen)) return false;

        out.x = std::min(out.x, screen.x);
        out.y = std::min(out.y, screen.y);
        out.w = std::max(out.w, screen.x);
        out.h = std::max(out.h, screen.y);
    }

    return true;
}

bool C_BaseEntity::ComputeHitboxSurroundingBox(Vector& mins, Vector& maxs) {
    if (!memory::fnComputeHitboxSurroundingBox) return false;
    return memory::fnComputeHitboxSurroundingBox(this, mins, maxs);
}

float C_BaseEntity::DistanceToSquared(C_BaseEntity* pEntity) {
    const Vector& currentOrigin = GetOrigin();
    const Vector& entityOrigin = pEntity->GetOrigin();

    return currentOrigin.DistToSquaredInMeters(entityOrigin);
}
