#include "pch.hpp"

#include <hacks/esp/esp.hpp>

#include <cache/cache.hpp>
#include <vars/vars.hpp>

#include <interfaces/engineclient.hpp>

bool CESP::IsEnabled() { return g_Vars.m_EnableESP && CEngineClient::Get()->IsInGame(); }

void CESP::Render() {
    const std::lock_guard<std::mutex> lock(CMatchCache::GetLock());

    const auto& cachedEntities = CMatchCache::GetCachedEntities();
    for (const auto& it : cachedEntities) {
        const auto& cachedEntity = it.second;
        if (!cachedEntity->CanDoESP() || !IsEnabled()) {
            cachedEntity->InvalidateDrawInfo();
            continue;
        }

        cachedEntity->DrawESP();
    }
}

void CESP::Update() {
    const std::lock_guard<std::mutex> lock(CMatchCache::GetLock());

    const auto& cachedEntities = CMatchCache::GetCachedEntities();
    for (const auto& it : cachedEntities) {
        const auto& cachedEntity = it.second;
        if (!cachedEntity->CanDoESP() || !IsEnabled()) {
            cachedEntity->InvalidateDrawInfo();
            continue;
        }

        cachedEntity->CalculateDrawInfo();
    }
}
