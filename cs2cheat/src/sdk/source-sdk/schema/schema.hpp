#pragma once

#include <memory>

#include <fnv1a/hash_fnv1a_constexpr.h>

#define SCHEMA_FIELD_OFFSET(varName, datatable, propName, extra_offset, type)  \
    std::add_lvalue_reference_t<type> varName() {                              \
        static constexpr auto datatable_hash = hash_32_fnv1a_const(datatable); \
        static constexpr auto prop_hash = hash_32_fnv1a_const(propName);       \
                                                                               \
        static const auto m_offset =                                           \
            schema::GetOffset(datatable, datatable_hash, propName, prop_hash); \
                                                                               \
        return *reinterpret_cast<std::add_pointer_t<type>>(                    \
            (uintptr_t)(this) + m_offset + extra_offset);                      \
    }

#define SCHEMA_FIELD(varName, datatable, propName, type) \
    SCHEMA_FIELD_OFFSET(varName, datatable, propName, 0, type)

#define PSCHEMA_FIELD_OFFSET(varName, datatable, propName, extra_offset, type) \
    auto varName() {                                                           \
        static constexpr auto datatable_hash = hash_32_fnv1a_const(datatable); \
        static constexpr auto prop_hash = hash_32_fnv1a_const(propName);       \
                                                                               \
        static const auto m_offset =                                           \
            schema::GetOffset(datatable, datatable_hash, propName, prop_hash); \
                                                                               \
        return reinterpret_cast<std::add_pointer_t<type>>(                     \
            (uintptr_t)(this) + m_offset + extra_offset);                      \
    }

#define PSCHEMA_FIELD(varName, datatable, propName, type) \
    PSCHEMA_FIELD_OFFSET(varName, datatable, propName, 0, type)

namespace schema {
    int16_t GetOffset(const char* className, uint32_t classKey,
                      const char* memberName, uint32_t memberKey);
}
