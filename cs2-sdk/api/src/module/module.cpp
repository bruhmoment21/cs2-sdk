#include "pch.hpp"

#include <module/module.hpp>

#include <pointer/pointer.hpp>

#include <fnv/fnv1a.hpp>

CModule::CModule(const char* name) {
    m_Name = name;
    m_Hash = fnv1a::Hash(name);
    m_Handle = nullptr;

    m_Begin = m_Size = 0;
}

CModule::~CModule() {
    if (!m_Handle) {
        return;
    }

#ifndef _WIN32
    dlclose(m_Handle);
#endif
}

bool CModule::Retrieve() {
    if (m_Handle) {
        return true;
    }

    InitializeHandle();
    InitializeBounds();

    return m_Handle != nullptr;
}

uintptr_t CModule::GetInterface(const char* version) {
    uintptr_t rv = 0;
    if (m_Handle) {
        CPointer pCreateInterface = GetProcAddress("CreateInterface");
        if (!pCreateInterface.IsValid()) {
            return rv;
        }

        // Used internally to register classes.
        struct InterfaceReg {
            std::add_pointer_t<uintptr_t()> m_CreateFn;
            const char* m_pName;
            InterfaceReg* m_pNext;  // For the global list.
        };

        InterfaceReg* s_pInterfaceRegs =
#ifdef _WIN32
            pCreateInterface.Absolute(3, 0).Dereference(1).Get<InterfaceReg*>()
#elif __linux__
            pCreateInterface.Absolute(1, 0).Absolute(19, 0).Dereference(1).Get<InterfaceReg*>()
#endif
            ;

        uint32_t versionHash = fnv1a::Hash(version);
        for (; s_pInterfaceRegs; s_pInterfaceRegs = s_pInterfaceRegs->m_pNext) {
            if (fnv1a::Hash(s_pInterfaceRegs->m_pName) == versionHash) {
                rv = s_pInterfaceRegs->m_CreateFn();
                break;
            }
        }
    }

    return rv;
}

uintptr_t CModule::GetProcAddress(const char* procName) {
    uintptr_t rv = 0;
    if (m_Handle) {
#ifdef _WIN32
        rv = reinterpret_cast<uintptr_t>(::GetProcAddress(static_cast<HMODULE>(m_Handle), procName));
#else
        rv = reinterpret_cast<uintptr_t>(dlsym(m_Handle, procName));
#endif
    }

    return rv;
}

uintptr_t CModule::FindPattern(const std::span<const int>& pattern) const {
    uintptr_t rv = 0;
    if (m_Handle) {
        uint8_t* bytes = reinterpret_cast<uint8_t*>(m_Begin);
        for (size_t i = 0; i < m_Size - pattern.size(); ++i) {
            bool found = true;
            for (size_t j = 0; j < pattern.size(); ++j) {
                if (bytes[i + j] != pattern[j] && pattern[j] != -1) {
                    found = false;
                    break;
                }
            }

            if (found) {
                rv = reinterpret_cast<uintptr_t>(&bytes[i]);
                break;
            }
        }
    }

    return rv;
}

void CModule::InitializeHandle() {
#ifdef _WIN32
    m_Handle = GetModuleHandle(GetName());
#else
    m_Handle = dlopen(GetName(), RTLD_LAZY | RTLD_NOLOAD);
#endif
}

void CModule::InitializeBounds() {
    if (!m_Handle) {
        return;
    }

#ifdef _WIN32
    MODULEINFO mi;
    BOOL status = GetModuleInformation(GetCurrentProcess(), static_cast<HMODULE>(m_Handle), &mi, sizeof(mi));
    if (status != 0) {
        SetBounds(reinterpret_cast<uintptr_t>(m_Handle), static_cast<uintptr_t>(mi.SizeOfImage));
    }
#elif __linux__
    dl_iterate_phdr(
        [](struct dl_phdr_info* info, size_t, void* data) {
            CModule* currentModule = static_cast<CModule*>(data);
            if (strstr(info->dlpi_name, currentModule->GetName()) != 0) {
                for (Elf64_Half i = 0; i < info->dlpi_phnum; ++i) {
                    const auto& phdr = info->dlpi_phdr[i];
                    if (phdr.p_type != PT_LOAD || !(phdr.p_flags & PF_X)) continue;

                    currentModule->SetBounds(info->dlpi_addr + phdr.p_vaddr, phdr.p_memsz);
                    return 1;
                }
            }

            return 0;
        },
        static_cast<void*>(this));
#endif
}
