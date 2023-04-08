#pragma once

#include "../source-sdk/classes/interfaces/cgameresourceserviceclient.hpp"
#include "../source-sdk/classes/interfaces/cgameentitysystem.hpp"
#include "../source-sdk/classes/interfaces/csource2client.hpp"
#include "../source-sdk/classes/interfaces/cschemasystem.hpp"
#include "../source-sdk/classes/interfaces/cengineclient.hpp"
#include "../source-sdk/classes/interfaces/cinputsystem.hpp"
#include "../source-sdk/classes/interfaces/clocalize.hpp"
#include "../source-sdk/classes/interfaces/ccvar.hpp"

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

    // Unexported interfaces:
    inline CGameEntitySystem* pEntitySystem;
}  // namespace interfaces
