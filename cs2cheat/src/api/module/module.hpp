#pragma once

#include <string>
#include <array>

// Windows API
#include <Windows.h>
#include <Psapi.h>

#include "../../sdk/source-sdk/classes/interface.hpp"
#include "../../console/console.hpp"
#include "../../defines.hpp"

// Pointer Class
struct UTILPtr {
   public:
    template <typename T>
    UTILPtr(const T& val) {
        m_val = (uintptr_t)(val);
    }

    template <typename T>
    T GetAs() {
        return (T)(m_val);
    }

    UTILPtr& AddOffset(int offset) {
        if (m_val) m_val += offset;
        return *this;
    }
    UTILPtr& ToAbsolute(int preOffset, int postOffset) {
        if (m_val) {
            AddOffset(preOffset);
            m_val = m_val + sizeof(int) + *(int*)(m_val);
            AddOffset(postOffset);
        }
        return *this;
    }
    UTILPtr& Dereference(int dereferences) {
        if (m_val)
            while (dereferences-- != 0) m_val = *(uintptr_t*)(m_val);
        return *this;
    }

   private:
    uintptr_t m_val;
};

class CModule {
   public:
    // CS2TODO: Maybe write a simple caching system

    CModule(CModule&&) = delete;
    CModule(const CModule&) = delete;

    explicit CModule(const char* name, bool shouldLoad) {
        this->m_name = name;
        if (shouldLoad) Load();
    }
    void Load() {
        InitializeHandle();
        InitializeBounds();
    }

    void* GetProcAddress(const char* procName) const {
        void* rv = nullptr;
        if (this->IsLoaded()) {
#ifdef IS_WINDOWS
            rv = static_cast<void*>(::GetProcAddress(
                static_cast<HMODULE>(this->m_handle), procName));
#endif
        }
        return rv;
    }
    template <typename T = void*>
    T FindInterfaceAs(const char* version) const {
        void* rv = nullptr;
        if (this->IsLoaded()) {
            UTILPtr pCreateInterface = this->GetProcAddress("CreateInterface");
            InterfaceReg* s_pInterfaceRegs = pCreateInterface.ToAbsolute(3, 0)
                                                 .Dereference(1)
                                                 .GetAs<InterfaceReg*>();

            for (; s_pInterfaceRegs;
                 s_pInterfaceRegs = s_pInterfaceRegs->m_pNext) {
                if (strcmp(version, s_pInterfaceRegs->m_pName) == 0) {
                    rv = s_pInterfaceRegs->m_CreateFn();
                    break;
                }
            }
        }
        return (T)(rv);
    }
    template <size_t N>
    UTILPtr FindPattern(const std::array<int, N>& signature) const {
        UTILPtr rv = 0;
        if (this->IsLoaded()) {
            uint8_t* pBytes = reinterpret_cast<uint8_t*>(this->m_start);
            for (size_t i = 0; i < this->m_end - N; ++i) {
                bool found = true;
                for (size_t j = 0; j < N; ++j) {
                    if (pBytes[i + j] != signature[j] && signature[j] != -1) {
                        found = false;
                        break;
                    }
                }

                if (found) {
                    rv = reinterpret_cast<uintptr_t>(&pBytes[i]);
                    break;
                }
            }
        }
        return rv;
    }
    const char* GetName() const { return this->m_name.c_str(); }
    bool IsLoaded() const { return this->m_handle != 0; }

   private:
    void InitializeHandle() {
#ifdef IS_WINDOWS
        this->m_handle = static_cast<void*>(GetModuleHandle(this->GetName()));
#endif
    }
    void InitializeBounds() {
        if (!this->IsLoaded()) return;

#ifdef IS_WINDOWS
        MODULEINFO mi;
        BOOL status = GetModuleInformation(GetCurrentProcess(),
                                           static_cast<HMODULE>(this->m_handle),
                                           &mi, sizeof(mi));
        if (status != 0) {
            this->m_start = reinterpret_cast<uintptr_t>(this->m_handle);
            this->m_end = static_cast<uintptr_t>(mi.SizeOfImage);
        }
#endif
    }

    void* m_handle = nullptr;
    uintptr_t m_start = 0, m_end = 0;
    std::string m_name = "";
};
