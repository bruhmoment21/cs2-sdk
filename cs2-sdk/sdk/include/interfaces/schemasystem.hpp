#pragma once

class CSchemaSystemTypeScope;
class CSchemaClassInfo;
class CSchemaBaseClassInfo;
class CSchemaClassField;

class CSchemaSystem {
   public:
    static CSchemaSystem* Get();

    CSchemaSystemTypeScope* FindTypeScopeForModule(const char* name);
};

class CSchemaSystemTypeScope {
   public:
    CSchemaClassInfo* FindDeclaredClass(const char* name);
};

struct SchemaClassInfoData_t {
    char pad0[0x8];                       // 0x00
    const char* m_Name;                   // 0x08
    const char* m_Description;            // 0x10
    int32_t m_nSizeOf;                    // 0x18
    uint8_t m_nFieldsCount;               // 0x1C
    char pad1[0x5];                       // 0x1D
    uint8_t m_nAlignOf;                   // 0x22
    uint8_t m_nBaseClassesCount;          // 0x23
    char pad2[0x4];                       // 0x24
    CSchemaClassField* m_Fields;          // 0x28
    char pad3[0x8];                       // 0x30
    CSchemaBaseClassInfo* m_BaseClasses;  // 0x38
    char pad4[0x28];                      // 0x40
};

class CSchemaClassInfo : public SchemaClassInfoData_t {
   public:
    // Full implementation here:
    // https://github.com/neverlosecc/source2gen/blob/fc4f43dfc65dd47c9eb118ce1646a5ccf90a00f0/include/sdk/interfaces/schemasystem/schema.h#L267-L309

    bool InheritsFrom(CSchemaClassInfo* other);
};

struct SchemaBaseClassInfoData_t {
    int32_t m_nOffset;
    CSchemaClassInfo* m_pClass;
};

class CSchemaBaseClassInfo : public SchemaBaseClassInfoData_t {
   public:
};

struct SchemaClassFieldData_t {
    const char* m_Name;
    char pad0[0x8];
    int32_t m_nSingleInheritanceOffset;
    char pad1[0xC];
};

class CSchemaClassField : public SchemaClassFieldData_t {
   public:
};
