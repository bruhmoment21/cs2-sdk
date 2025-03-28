#pragma once

#include <pointer/pointer.hpp>

namespace vt {
    CPointer GetMethod(CPointer base, uint32_t index);

    /*
     *   CallMethod usage:
     *      CallMethod<ReturnedType>(thisPointer, methodIndex, arg1, arg2, arg3...);
     *
     *      It automatically deduces argument types except for references, usually you don't have those
     *      but you can pass the argument with reference using std::ref(arg4) <- Example.
     */
    template <typename T, typename... Args>
    inline T CallMethod(CPointer base, uint32_t index, Args... args) {
        return GetMethod(base, index).Call<T (*)(void*, Args...)>(base.Get<void*>(), std::forward<Args>(args)...);
    }
}  // namespace vt
