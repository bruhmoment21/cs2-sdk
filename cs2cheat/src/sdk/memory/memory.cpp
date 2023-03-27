#include "memory.hpp"

#include <stb/stb.hh>

#define LOG_RESULT(src) LOG("memory::%s found at -> %p\n", #src, src)

void memory::Initialize() {
    CModule client(CLIENT_DLL, true);
    CModule schemasystem(SCHEMASYSTEM_DLL, true);

    fnGetClientNetworkable = client.FindPattern(GET_CLIENT_NETWORKABLE)
                                 .ToAbsolute(3, 0)
                                 .GetAs<decltype(fnGetClientNetworkable)>();
    LOG_RESULT(fnGetClientNetworkable);
    fnGetMaxEntities = client.FindPattern(GET_MAX_ENTITIES)
                           .ToAbsolute(3, 0)
                           .GetAs<decltype(fnGetMaxEntities)>();
    LOG_RESULT(fnGetMaxEntities);
    schema_detailed_class_layout =
        schemasystem.FindPattern(PRINT_SCHEMA_DETAILED_CLASS_LAYOUT)
            .GetAs<decltype(schema_detailed_class_layout)>();
    LOG_RESULT(schema_detailed_class_layout);
    fnScreenTransform = client.FindPattern(SCREEN_TRANSFORM)
                            .ToAbsolute(1, 0)
                            .GetAs<decltype(fnScreenTransform)>();
    LOG_RESULT(fnScreenTransform);
}

void memory::Shutdown() {}
