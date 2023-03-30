#include "interfaces.hpp"

#include "../../api/module/module.hpp"

#define FIND_INTERFACE(dst, module, version)              \
    dst = module.FindInterfaceAs<decltype(dst)>(version); \
    LOG("interfaces::%s found at -> %p\n", #dst, dst)

void interfaces::Initialize() {
    CModule engine(ENGINE2_DLL, true);
    CModule client(CLIENT_DLL, true);
    CModule schemasystem(SCHEMASYSTEM_DLL, true);
    CModule inputsystem(INPUTSYSTEM_DLL, true);

    FIND_INTERFACE(pGameResourceService, engine, GAME_RESOURCE_SERVICE_CLIENT);
    FIND_INTERFACE(pClient, client, SOURCE2_CLIENT);
    FIND_INTERFACE(pSchemaSystem, schemasystem, SCHEMA_SYSTEM);
    FIND_INTERFACE(pInputSystem, inputsystem, INPUT_SYSTEM_VERSION);

    pEntitySystem = pGameResourceService->GetGameEntitySystem();
    LOG("interfaces::pEntitySystem found at -> %p\n", pEntitySystem);
}

void interfaces::Shutdown() {}
