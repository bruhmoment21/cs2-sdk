#include "interfaces.hpp"

#include "../../api/module/module.hpp"

#define FIND_INTERFACE(dst, module, version)            \
    dst = module.FindInterface<decltype(dst)>(version); \
    LOG("interfaces::%s found at -> %p\n", #dst, dst)

void interfaces::Initialize() {
    CModule engine(ENGINE2_DLL, true);
    CModule client(CLIENT_DLL, true);
    CModule schemasystem(SCHEMASYSTEM_DLL, true);
    CModule inputsystem(INPUTSYSTEM_DLL, true);
    CModule tier0(TIER0_DLL, true);
    CModule localize(LOCALIZE_DLL, true);

    FIND_INTERFACE(pGameResourceService, engine, GAME_RESOURCE_SERVICE_CLIENT);
    FIND_INTERFACE(pClient, client, SOURCE2_CLIENT);
    FIND_INTERFACE(pSchemaSystem, schemasystem, SCHEMA_SYSTEM);
    FIND_INTERFACE(pInputSystem, inputsystem, INPUT_SYSTEM_VERSION);
    FIND_INTERFACE(pEngine, engine, SOURCE2_ENGINE_TO_CLIENT);
    FIND_INTERFACE(pCvar, tier0, ENGINE_CVAR);
    FIND_INTERFACE(pLocalize, localize, LOCALIZE);

    pEntitySystem = pGameResourceService->GetGameEntitySystem();
    LOG("interfaces::pEntitySystem found at -> %p\n", pEntitySystem);
}

void interfaces::Shutdown() {}
