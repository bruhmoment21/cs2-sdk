#pragma once

#include "../../../virtual.hpp"

struct SchemaClassFieldData_t {
    const char* pName;
    char pad0[0x8];
    short uOffset;
    char pad1[0xE];
};

class SchemaClassInfoData_t {
   public:
    short GetFieldsSize() {
        return *reinterpret_cast<short*>((uintptr_t)(this) + 0x1C);
    }
    SchemaClassFieldData_t* GetFields() {
        return *reinterpret_cast<SchemaClassFieldData_t**>((uintptr_t)(this) +
                                                           0x28);
    }
};

class CSchemaSystemTypeScope {
   public:
    auto FindDeclaredClass(const char* pClass) {
        return CALL_VIRTUAL(SchemaClassInfoData_t*, 2, this, pClass);
    }
};

class CSchemaSystem {
   public:
    auto FindTypeScopeForModule(const char* module) {
        return CALL_VIRTUAL(CSchemaSystemTypeScope*, 13, this, module);
    }
};
