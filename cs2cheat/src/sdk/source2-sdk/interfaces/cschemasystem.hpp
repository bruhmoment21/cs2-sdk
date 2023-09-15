#pragma once

#include "../../virtual.hpp"

struct SchemaClassFieldData_t {
    const char* m_name;
    char pad0[0x8];
    short m_offset;
    char pad1[0xE];
};

class SchemaClassInfoData_t {
   public:
    auto GetName() {
        return *reinterpret_cast<const char**>((uintptr_t)(this) + 0x8);
    }

    auto GetFieldsSize() {
        return *reinterpret_cast<short*>((uintptr_t)(this) + 0x1C);
    }

    auto GetFields() {
        return *reinterpret_cast<SchemaClassFieldData_t**>((uintptr_t)(this) +
                                                           0x28);
    }
};

class CSchemaSystemTypeScope {
   public:
    auto FindDeclaredClass(const char* pClass) {
        SchemaClassInfoData_t* rv = nullptr;
        CALL_VIRTUAL(void, 2, this, &rv, pClass);
        return rv;
    }
};

class CSchemaSystem {
   public:
    auto FindTypeScopeForModule(const char* module) {
        return CALL_VIRTUAL(CSchemaSystemTypeScope*, 13, this, module, nullptr);
    }
};
