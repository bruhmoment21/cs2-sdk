#pragma once

#include "../source-sdk/classes/cgameresourceserviceclient.hpp"
#include "../source-sdk/classes/cgameentitysystem.hpp"
#include "../source-sdk/classes/csource2client.hpp"
#include "../source-sdk/classes/cschemasystem.hpp"
#include "../source-sdk/classes/cinputsystem.hpp"

namespace interfaces {
    void Initialize();
    void Shutdown();

    inline CGameResourceService* pGameResourceService;
    inline CSource2Client* pClient;
    inline CSchemaSystem* pSchemaSystem;
    inline CInputSystem* pInputSystem;

    // Unexported interfaces:
    inline CGameEntitySystem* pEntitySystem;
}  // namespace interfaces
