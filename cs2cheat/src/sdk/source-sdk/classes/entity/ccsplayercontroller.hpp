#pragma once

#include "cbaseplayercontroller.hpp"
#include "c_csplayerpawn.hpp"

class CCSPlayerController : public CBasePlayerController {
   public:
    SCHEMA_FIELD(m_sSanitizedPlayerName, "CCSPlayerController",
                 "m_sSanitizedPlayerName", const char*);
    SCHEMA_FIELD(m_iPawnHealth, "CCSPlayerController", "m_iPawnHealth",
                 uint32_t);
    SCHEMA_FIELD(m_bPawnIsAlive, "CCSPlayerController", "m_bPawnIsAlive", bool);
};
