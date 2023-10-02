#pragma once

struct Vars_t {
    bool m_EnableESP;

    bool m_PlayerBoxes;
    bool m_PlayerNames;
    bool m_PlayerHealthBar;

    bool m_WeaponESP;
    bool m_ChickenESP;
    bool m_OtherESP;
    bool m_Use3DBoxes;
};

extern Vars_t g_Vars;
