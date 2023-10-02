#pragma once

#include <constants/constants.hpp>
#include <pointer/pointer.hpp>

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
