#include "c_baseentity.hpp"

#include "../../../interfaces/interfaces.hpp"
#include "../../../memory/memory.hpp"

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

bool C_BaseEntity::GetBoundingBox(BBox_t& out) {
    CCollisionProperty* pCollision = m_pCollision();
    if (!pCollision) return false;

    CGameSceneNode* pGameSceneNode = m_pGameSceneNode();
    if (!pGameSceneNode) return false;

    const Vector& origin = pGameSceneNode->m_vecAbsOrigin();
    const Vector min = pCollision->m_vecMins() + origin;
    const Vector max = pCollision->m_vecMaxs() + origin;

    out.x = out.y = std::numeric_limits<float>::max();
    out.w = out.h = -std::numeric_limits<float>::max();

    for (size_t i = 0; i < 8; ++i) {
        const Vector points{i & 1 ? max.x : min.x, i & 2 ? max.y : min.y,
                            i & 4 ? max.z : min.z};
        Vector screen;
        if (!math::WorldToScreen(points, screen)) return false;

        out.x = IM_FLOOR(std::min(out.x, screen.x));
        out.y = IM_FLOOR(std::min(out.y, screen.y));
        out.w = IM_FLOOR(std::max(out.w, screen.x));
        out.h = IM_FLOOR(std::max(out.h, screen.y));
    }

    return true;
}
