#include "interfaces.hpp"

#include "../../api/module/module.hpp"

#define FIND_INTERFACE(dst, module, version) \
    module.FindInterface(version).Get(dst, "interfaces::" #dst);

void interfaces::Initialize() {
    CModule engine(ENGINE2_DLL);
    CModule client(CLIENT_DLL);
    CModule schemasystem(SCHEMASYSTEM_DLL);
    CModule inputsystem(INPUTSYSTEM_DLL);
    CModule tier0(TIER0_DLL);
    CModule localize(LOCALIZE_DLL);

    FIND_INTERFACE(pGameResourceService, engine, GAME_RESOURCE_SERVICE_CLIENT);
    FIND_INTERFACE(pClient, client, SOURCE2_CLIENT);
    FIND_INTERFACE(pSchemaSystem, schemasystem, SCHEMA_SYSTEM);
    FIND_INTERFACE(pInputSystem, inputsystem, INPUT_SYSTEM_VERSION);
    FIND_INTERFACE(pEngine, engine, SOURCE2_ENGINE_TO_CLIENT);
    FIND_INTERFACE(pCvar, tier0, ENGINE_CVAR);
    FIND_INTERFACE(pLocalize, localize, LOCALIZE);
}

void interfaces::Shutdown() {}
