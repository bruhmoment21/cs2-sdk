#pragma once

#include <string>
#include <array>

// Windows API
#include <Windows.h>
#include <Psapi.h>

#include "../../sdk/source2-sdk/interface.hpp"
#include "../../utils/console/console.hpp"
#include "../../defines.hpp"

// Pointer arithmetic utility class.
struct UTILPtr {
   public:
    template <typename T>
    UTILPtr(T val) {
        m_val = (uintptr_t)(val);
    }

    template <typename T = void*>
    T Get(const char* variableName = nullptr) {
#ifdef CS2_SDK_ENABLE_LOGGING
        if (variableName) LOG("%s found at -> %llX\n", variableName, m_val);
#endif

        return (T)(m_val);
    }

    template <typename T>
    void Get(T& dst, const char* variableName = nullptr) {
        dst = Get<T>(variableName);
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

    bool IsValid() { return m_val != 0; }

   private:
    uintptr_t m_val;
};

class CModule {
   public:
    // CS2TODO: Maybe write a simple caching system

    CModule(CModule&&) = delete;
    CModule(const CModule&) = delete;

    explicit CModule(const char* name) : m_name(name) { this->Load(); }

    void Load() {
        this->InitializeHandle();
        this->InitializeBounds();
    }

    UTILPtr GetProcAddress(const char* procName) const {
        UTILPtr rv = 0;
        if (this->IsLoaded()) {
#ifdef IS_WINDOWS
            rv = ::GetProcAddress(static_cast<HMODULE>(this->m_handle),
                                  procName);
#endif
        }

        LOG("GetProcAddress('%s', '%s') returned -> %p\n", this->GetName(),
            procName, rv.Get<void*>());
        return rv;
    }
    UTILPtr FindInterface(const char* version) const {
        UTILPtr rv = 0;
        if (this->IsLoaded()) {
            UTILPtr pCreateInterface = this->GetProcAddress("CreateInterface");
            if (!pCreateInterface.IsValid()) return rv;

            InterfaceReg* s_pInterfaceRegs = pCreateInterface.ToAbsolute(3, 0)
                                                 .Dereference(1)
                                                 .Get<InterfaceReg*>();

            for (; s_pInterfaceRegs;
                 s_pInterfaceRegs = s_pInterfaceRegs->m_pNext) {
                if (strcmp(version, s_pInterfaceRegs->m_pName) == 0) {
                    rv = s_pInterfaceRegs->m_CreateFn();
                    break;
                }
            }
        }
        return rv;
    }
    template <size_t N>
    UTILPtr FindPattern(const std::array<int, N>& signature) const {
        UTILPtr rv = 0;
        if (this->IsLoaded()) {
            const int* pSigData = signature.data();
            uint8_t* pBytes = reinterpret_cast<uint8_t*>(this->m_start);
            for (size_t i = 0; i < this->m_end - N; ++i) {
                bool found = true;
                for (size_t j = 0; j < N; ++j) {
                    if (pBytes[i + j] != pSigData[j] && pSigData[j] != -1) {
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
    const char* GetName() const { return this->m_name; }
    bool IsLoaded() const { return this->m_handle != 0; }

   private:
    void InitializeHandle() {
#ifdef IS_WINDOWS
        this->m_handle = static_cast<void*>(GetModuleHandle(this->GetName()));
#endif

        LOG("Module '%s' found at -> %p\n", this->GetName(), this->m_handle);
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
    const char* m_name = "";
};
