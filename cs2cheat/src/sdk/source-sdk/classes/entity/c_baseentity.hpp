#pragma once

#include "centityinstance.hpp"
#include "cgamescenenode.hpp"

class C_BaseEntity : public CEntityInstance {
   public:
    bool IsPlayerController() {
        CEntityIdentity* pIdentity = m_pEntity();
        if (!pIdentity) return false;

        const char* designerName = pIdentity->m_designerName();
        if (!designerName) return false;

        // CS2TODO: Maybe use hashes?
        return strcmp(designerName, "cs_player_controller") == 0;
    }

    NETVAR(m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode",
           CGameSceneNode*);
};
