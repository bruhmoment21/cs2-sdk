#pragma once

#include "c_baseentity.hpp"

class CBasePlayerController : public C_BaseEntity {
   public:
    SCHEMA_FIELD(m_steamID, "CBasePlayerController", "m_steamID", uint64_t);
    SCHEMA_FIELD(m_hPawn, "CBasePlayerController", "m_hPawn", CHandle);
    SCHEMA_FIELD(m_bIsLocalPlayerController, "CBasePlayerController",
                 "m_bIsLocalPlayerController", bool);
};
