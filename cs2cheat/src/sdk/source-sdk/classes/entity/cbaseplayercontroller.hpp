#pragma once

#include "c_baseentity.hpp"

class CBasePlayerController : public C_BaseEntity {
   public:
    NETVAR(m_steamID, "CBasePlayerController", "m_steamID", uint64_t);
};
