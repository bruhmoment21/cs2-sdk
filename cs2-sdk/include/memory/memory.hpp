#pragma once

#include <module/module.hpp>
#include <pointer/pointer.hpp>

class CSigScan;

using ModulePtr_t = std::unique_ptr<CModule>;

class CMemory {
   public:
    static CMemory& Get() {
        static CMemory inst;
        return inst;
    }

    static auto& GetModule(const char* libName) { return Get().GetModuleInternal(libName); }
    static auto GetInterface(const char* libName, const char* version) { return Get().GetInterfaceInternal(libName, version); }
    static auto GetProcAddress(const char* libName, const char* procName) { return Get().GetProcAddressInternal(libName, procName); }

    // Used internally.
    static auto ScheduleScan(CSigScan* sigScan) { Get().m_ScheduledScans.emplace_back(sigScan); }

    void Initialize();

   private:
    ModulePtr_t& GetModuleInternal(const char* libName);

    CPointer GetInterfaceInternal(const char* libName, const char* version);
    CPointer GetProcAddressInternal(const char* libName, const char* procName);

    std::unordered_map<uint32_t, ModulePtr_t> m_CachedModules;
    std::vector<CSigScan*> m_ScheduledScans;
};
