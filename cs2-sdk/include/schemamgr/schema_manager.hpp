#pragma once

#include <constants/constants.hpp>
#include <pointer/pointer.hpp>

/*
 *   Basic usage of schemas:
 *
 *      1. SCHEMA(Type, fieldName, "ClassToFindField", "fieldNameInClass");
 *      2. SCHEMA_EXTENDED(Type, fieldName, "ClassToFindField", "fieldNameInClass", extraOffset);
 *
 *          PSCHEMA and PSCHEMA_EXTENDED has the same usage as SCHEMA but for pointers.
 *
 *          SCHEMA_EXTENDED is used for specific fields that don't explicitly appear
 *           in the CSchemaClassInfo* but we know they exist so we will find them
 *           through another field that exists.
 *           Example:
 *               class BaseEntity {
 *                   ...                     // 0xFFF
 *                   int m_SomeValue;        // 0x1000
 *                   int m_SomeHiddenValue;  // 0x1004
 *               };
 *              m_SomeHiddenValue doesn't appear in the CSchemaClassInfo* of class but we know its below
 *              m_SomeValue that appears in the CSchemaClassInfo* and its just below it so right after 4 bytes.
 *              Usage: SCHEMA_EXTENDED(int, m_SomeHiddenValue, "BaseEntity", "m_SomeValue", 4);
 */

class CSchemaManager {
   public:
    static CSchemaManager& Get() {
        static CSchemaManager inst;
        return inst;
    }

    static auto GetSchemaOffset(const char* moduleName, const char* bindingName, const char* fieldName) {
        return Get().GetSchemaOffsetInternal(moduleName, bindingName, fieldName);
    }

   private:
    std::optional<int32_t> GetSchemaOffsetInternal(const char* moduleName, const char* bindingName, const char* fieldName);
};

#define SCHEMA_EXTENDED(type, name, module_name, binding_name, field_name, extra_offset)                   \
    std::add_lvalue_reference_t<type> name() const {                                                       \
        static const auto offset = CSchemaManager::GetSchemaOffset(module_name, binding_name, field_name); \
        if (offset) return CPointer(this).GetField<type>(offset.value() + extra_offset);                   \
        static type null{};                                                                                \
        return null;                                                                                       \
    }

#define SCHEMA(type, name, binding_name, field_name) SCHEMA_EXTENDED(type, name, CConstants::CLIENT_LIB, binding_name, field_name, 0)

#define PSCHEMA_EXTENDED(type, name, module_name, binding_name, field_name, extra_offset)                             \
    std::add_pointer_t<type> name() const {                                                                           \
        static const auto offset = CSchemaManager::GetSchemaOffset(module_name, binding_name, field_name);            \
        if (offset) return CPointer(this).GetFieldPtr(offset.value() + extra_offset).Get<std::add_pointer_t<type>>(); \
        return nullptr;                                                                                               \
    }

#define PSCHEMA(type, name, binding_name, field_name) PSCHEMA_EXTENDED(type, name, CConstants::CLIENT_LIB, binding_name, field_name, 0)
