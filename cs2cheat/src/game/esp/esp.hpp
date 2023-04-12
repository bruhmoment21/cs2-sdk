#pragma once

namespace esp {
    void Render();

    inline bool bBoxes, bName, bHealthbar, bIgnoreTeammates, bActiveWeaponName,
        bDroppedWeaponBoxes, bDroppedWeaponName;
    inline float fWeaponMaxDistance;
    inline bool bChickenBox;
}  // namespace esp
