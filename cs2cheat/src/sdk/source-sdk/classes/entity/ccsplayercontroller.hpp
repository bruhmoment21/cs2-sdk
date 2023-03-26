#pragma once

#include "cbaseplayercontroller.hpp"

class CCSPlayerController : public CBasePlayerController {
   public:
    NETVAR(m_sSanitizedPlayerName, "CCSPlayerController",
           "m_sSanitizedPlayerName", const char*);
    NETVAR(m_iPawnHealth, "CCSPlayerController", "m_iPawnHealth", uint32_t);
};
