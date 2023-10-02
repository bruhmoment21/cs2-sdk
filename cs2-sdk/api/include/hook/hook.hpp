#pragma once

#define SDK_HOOK(hook) reinterpret_cast<void*>(hook), #hook

#include <pointer/pointer.hpp>

#include <funchook.h>

class CHook {
   public:
    void Hook(void* target, void* hook, const char* name = nullptr);
    void VHook(void* base, int index, void* hook, const char* name = nullptr);

    template <typename T, typename... Args>
    auto CallOriginal(Args... args) {
        return CPointer(m_Original).Call<T (*)(Args...)>(std::forward<Args>(args)...);
    }

    static funchook_t* s_FuncHookContext;

   private:
    void* m_Original;
};
