#include "pch.hpp"

#include <interfaces/engineclient.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

#include <virtual/virtual.hpp>

CEngineClient* CEngineClient::Get() {
    static const auto inst = CMemory::GetInterface(CConstants::ENGINE_LIB, "Source2EngineToClient001");
    return inst.Get<CEngineClient*>();
}

bool CEngineClient::IsInGame() { return vt::CallMethod<bool>(this, 31); }

int CEngineClient::GetLocalPlayer() {
    int index = -1;

#if _WIN32
    vt::CallMethod<void>(this, 43, &index, 0);
#elif __linux__
    index = vt::CallMethod<int>(this, 43, 0);
#endif

    return index + 1;
}
