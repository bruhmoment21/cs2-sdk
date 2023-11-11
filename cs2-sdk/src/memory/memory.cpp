#include "pch.hpp"

#include <memory/memory.hpp>
#include <logger/logger.hpp>

#include <sigscan/sigscan.hpp>

#include <fnv/fnv1a.hpp>

void CMemory::Initialize() {
    SDK_LOG_PROLOGUE();

    CSigScanManager::Get().ProcessScans();
}

ModulePtr_t& CMemory::GetModuleInternal(const char* libName) {
    auto hash = FNV1A::Hash(libName);

    auto it = m_CachedModules.find(hash);
    if (it != m_CachedModules.end()) {
        return it->second;
    }

    auto module = std::make_unique<CModule>(libName);
    if (module->Retrieve()) {
        CLogger::Log("[module] {} -> {}", libName, module->GetHandle());
        return m_CachedModules.emplace(hash, std::move(module)).first->second;
    }

    CLogger::Log("[module] {} not found.", libName);

    static ModulePtr_t null{};
    return null;
}

CPointer CMemory::GetInterfaceInternal(const char* libName, const char* version) {
    CPointer rv = 0;

    auto& library = CMemory::GetModuleInternal(libName);
    if (!library) {
        CLogger::Log("[interface] Couldn't retrieve {} because {} was not loaded", version, libName);
        return rv;
    }

    rv = library->GetInterface(FNV1A::Hash(version));
    CLogger::Log("[interface] {} -> {} -> {}", libName, version, rv.Get<void*>());

    return rv;
}

CPointer CMemory::GetProcAddressInternal(const char* libName, const char* procName) {
    CPointer rv = 0;

    auto& library = CMemory::GetModuleInternal(libName);
    if (!library) {
        CLogger::Log("[export] Couldn't retrieve {} because {} was not loaded.", procName, libName);
        return rv;
    }

    rv = library->GetProcAddress(procName);
    CLogger::Log("[export] {} -> {} -> {}", libName, procName, rv.Get<void*>());

    return rv;
}
