#pragma once

#include "c_basemodelentity.hpp"

class CBasePlayerController : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_steamID, "CBasePlayerController", "m_steamID", uint64_t);
    SCHEMA_FIELD(m_hPawn, "CBasePlayerController", "m_hPawn", CHandle);
    SCHEMA_FIELD(m_bIsLocalPlayerController, "CBasePlayerController",
                 "m_bIsLocalPlayerController", bool);
};
