#pragma once

#include "../source2-sdk/interfaces/cgameresourceserviceclient.hpp"
#include "../source2-sdk/interfaces/cgameentitysystem.hpp"
#include "../source2-sdk/interfaces/csource2client.hpp"
#include "../source2-sdk/interfaces/cschemasystem.hpp"
#include "../source2-sdk/interfaces/cengineclient.hpp"
#include "../source2-sdk/interfaces/cinputsystem.hpp"
#include "../source2-sdk/interfaces/cgameevent.hpp"
#include "../source2-sdk/interfaces/clocalize.hpp"
#include "../source2-sdk/interfaces/ccvar.hpp"

namespace interfaces {
    void Initialize();
    void Shutdown();

    inline CGameResourceService* pGameResourceService;
    inline CSource2Client* pClient;
    inline CSchemaSystem* pSchemaSystem;
    inline CInputSystem* pInputSystem;
    inline CEngineClient* pEngine;
    inline CCvar* pCvar;
    inline CLocalize* pLocalize;
}  // namespace interfaces
