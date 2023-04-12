#include "c_baseentity.hpp"

#include "../../interfaces/interfaces.hpp"
#include "../../memory/memory.hpp"

#include <imgui/imgui_internal.h>

bool C_BaseEntity::IsPlayerController() {
    CEntityIdentity* pIdentity = m_pEntity();
    if (!pIdentity) return false;

    const char* designerName = pIdentity->m_designerName();
    if (!designerName) return false;

    static constexpr auto cs_player_controller =
        hash_32_fnv1a_const("cs_player_controller");
    return hash_32_fnv1a_const(designerName) == cs_player_controller;
}

bool C_BaseEntity::IsWeapon() {
    CEntityIdentity* pIdentity = m_pEntity();
    if (!pIdentity) return false;

    const char* designerName = pIdentity->m_designerName();
    if (!designerName) return false;

    const std::string_view svDesignerName = designerName;
    return svDesignerName.starts_with("weapon_");
}

bool C_BaseEntity::IsChicken() {
    SchemaClassInfoData_t* pClassInfo = Schema_DynamicBinding();
    if (!pClassInfo) return false;

    const char* className = pClassInfo->GetName();
    if (!className) return false;

    static constexpr auto C_Chicken = hash_32_fnv1a_const("C_Chicken");
    return hash_32_fnv1a_const(className) == C_Chicken;
}

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

    for (size_t i = 0; i < 8; ++i) {
        const Vector point{i & 1 ? max.x : min.x, i & 2 ? max.y : min.y,
                           i & 4 ? max.z : min.z};
        Vector screen;
        if (!math::WorldToScreen(point, screen)) return false;

        out.x = IM_FLOOR(std::min(out.x, screen.x));
        out.y = IM_FLOOR(std::min(out.y, screen.y));
        out.w = IM_FLOOR(std::max(out.w, screen.x));
        out.h = IM_FLOOR(std::max(out.h, screen.y));
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
