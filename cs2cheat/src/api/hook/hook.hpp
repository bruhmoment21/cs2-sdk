#pragma once

#include <memory>

#include <funchook/src/funchook.h>

#include "../../sdk/virtual.hpp"

#define HOOK_FUNCTION(hook) hook, #hook

inline funchook_t *g_funchookCtx = nullptr;
inline bool g_isShuttingDown = false;

template <typename T>
class CHook {
   public:
    // Template has been used to avoid casts.
    template <typename OriginalT, typename HookT>
    void Hook(OriginalT _pOriginalFn, HookT &pHookFn, const char *szHookName) {
        if (this->m_pOriginalFn) {
            LOG("%s tried rehooking.\n", szHookName);
            return;
        }

        void *pOriginalFn = static_cast<void *>(_pOriginalFn);
        if (!pOriginalFn) {
            LOG("%s tried hooking null.\n", szHookName);
            return;
        }

        this->m_pOriginalFn =
            reinterpret_cast<decltype(this->m_pOriginalFn)>(pOriginalFn);
        int rv = funchook_prepare(
            g_funchookCtx, reinterpret_cast<void **>(&this->m_pOriginalFn),
            reinterpret_cast<void *>(pHookFn));

        if (rv == FUNCHOOK_ERROR_SUCCESS) {
            LOG("%s hooked successfully. [ %p -> %p ]\n", szHookName,
                pOriginalFn, pHookFn);
        } else {
            this->m_pOriginalFn = nullptr;
            LOG("%s hook failed. [ %s ]\n", szHookName,
                funchook_error_message(g_funchookCtx));
        }
    }

    template <typename HookT>
    void HookVirtual(void *pClass, int index, HookT &pHookFn,
                     const char *szHookName) {
        this->Hook(vmt::GetVMethod(index, pClass), pHookFn, szHookName);
    }

    std::add_pointer_t<T> m_pOriginalFn;
};
