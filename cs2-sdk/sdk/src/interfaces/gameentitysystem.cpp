#include "pch.hpp"

#include <interfaces/gameentitysystem.hpp>
#include <interfaces/gameresourceservice.hpp>

#include <signatures/signatures.hpp>

CGameEntitySystem* CGameEntitySystem::Get() { return CGameResourceService::Get()->GetGameEntitySystem(); }

C_BaseEntity* CGameEntitySystem::GetBaseEntityInternal(int index) {
    return signatures::GetBaseEntity.GetPtr().Call<C_BaseEntity* (*)(void*, int)>(this, index);
}

int CGameEntitySystem::GetHighestEntityIndexInternal() {
    int highestIdx = -1;

#ifdef _WIN32
    signatures::GetHighestEntityIndex.GetPtr().Call<void (*)(void*, int*)>(this, &highestIdx);
#elif __linux__
    highestIdx = signatures::GetHighestEntityIndex.GetPtr().Call<int (*)(void*)>(this);
#endif

    return highestIdx;
}
