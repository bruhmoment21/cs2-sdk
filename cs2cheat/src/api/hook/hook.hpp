#pragma once

#include <memory>

#include "../../sdk/virtual.hpp"

#include <funchook/src/funchook.h>

#define HOOK_FUNCTION(hook) hook, #hook

inline funchook_t *g_funchookCtx = nullptr;
inline bool g_isShuttingDown = false;

template <typename T>
class CHook {
    std::add_pointer_t<T> m_pOriginal;

   public:
    // Template has been used to avoid casts.
    template <typename OriginalT, typename HookT>
    void Hook(OriginalT _pOriginalFn, HookT &pHookFn, const char *szHookName) {
        if (this->m_pOriginal) {
            LOG("%s tried rehooking.\n", szHookName);
            return;
        }

        void *pOriginalFn = static_cast<void *>(_pOriginalFn);
        if (!pOriginalFn) {
            LOG("%s tried hooking null.\n", szHookName);
            return;
        }

        this->m_pOriginal =
            reinterpret_cast<decltype(this->m_pOriginal)>(pOriginalFn);
        int rv = funchook_prepare(g_funchookCtx,
                                  reinterpret_cast<void **>(&this->m_pOriginal),
                                  reinterpret_cast<void *>(pHookFn));

        if (rv == FUNCHOOK_ERROR_SUCCESS) {
            LOG("%s hooked successfully. [ %p -> %p ]\n", szHookName,
                pOriginalFn, pHookFn);
        } else {
            this->m_pOriginal = nullptr;
            LOG("%s hook failed. [ %s ]\n", szHookName,
                funchook_error_message(g_funchookCtx));
        }
    }

    template <typename HookT>
    void HookVirtual(void *pClass, int index, HookT &pHookFn,
                     const char *szHookName) {
        this->Hook(vmt::GetVMethod(index, pClass), pHookFn, szHookName);
    }

    // Shorthand for calling original.
    template <typename... Args>
    auto operator()(Args &&...args) {
        return std::invoke(this->m_pOriginal, std::forward<Args>(args)...);
    }
};
