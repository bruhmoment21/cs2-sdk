#pragma once

#include <memory>

#include <fnv1a/hash_fnv1a_constexpr.h>

#define NETVAR_OFFSET(varName, datatable, propName, extra_offset, type)        \
    std::add_lvalue_reference_t<type> varName() {                              \
        static constexpr auto datatable_hash = hash_32_fnv1a_const(datatable); \
        static constexpr auto prop_hash = hash_32_fnv1a_const(propName);       \
                                                                               \
        static const auto m_offset = netvars::GetOffset(                       \
            datatable, datatable_hash, propName, prop_hash);                   \
                                                                               \
        return *reinterpret_cast<std::add_pointer_t<type>>(                    \
            (uintptr_t)(this) + m_offset + extra_offset);                      \
    }

#define NETVAR(varName, datatable, propName, type) \
    NETVAR_OFFSET(varName, datatable, propName, 0, type)

namespace netvars {
    uintptr_t GetOffset(const char* className, uint32_t classKey,
                        const char* memberName, uint32_t memberKey);
}
