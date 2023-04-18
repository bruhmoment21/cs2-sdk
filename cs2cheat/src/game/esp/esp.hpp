#pragma once

#include "../../sdk/source2-sdk/types/chandle.hpp"

class CEntityInstance;

namespace esp {
    void Render();

    void CalculateBoundingBoxes();
    void CacheCurrentEntities();
    void OnAddEntity(CEntityInstance* pInst, CHandle handle);
    void OnRemoveEntity(CEntityInstance* pInst, CHandle handle);

    inline bool bBoxes, bName, bHealthbar, bIgnoreTeammates, bActiveWeaponName,
        bDroppedWeaponBoxes, bDroppedWeaponName, bIgnoreEnemies, bIgnoreSelf;
    inline float fWeaponMaxDistance;
    inline bool bChickenBox;
}  // namespace esp
