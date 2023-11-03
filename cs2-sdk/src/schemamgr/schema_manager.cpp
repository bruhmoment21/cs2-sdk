#include "pch.hpp"

#include <schemamgr/schema_manager.hpp>
#include <logger/logger.hpp>

#include <interfaces/schemasystem.hpp>

#include <fnv/fnv1a.hpp>

std::optional<int32_t> CSchemaManager::GetSchemaOffsetInternal(const char* moduleName, const char* bindingName, const char* fieldName) {
    CSchemaSystemTypeScope* typeScope = CSchemaSystem::Get()->FindTypeScopeForModule(moduleName);
    if (!typeScope) {
        CLogger::Log("[schemamgr] No type scope found for '{}'.", moduleName);
        return {};
    }

    CSchemaClassInfo* classInfo = typeScope->FindDeclaredClass(bindingName);
    if (!classInfo) {
        CLogger::Log("[schemamgr] No binding named '{}' has been found in module '{}'.", bindingName, moduleName);
        return {};
    }

    uint32_t fieldHash = FNV1A::Hash(fieldName);
    for (int i = 0; classInfo->m_Fields && i < classInfo->m_nFieldsCount; ++i) {
        auto& field = classInfo->m_Fields[i];
        if (FNV1A::Hash(field.m_Name) == fieldHash) {
            CLogger::Log("[schemamgr] '{}::{}' offset is 0x{:x}.", bindingName, fieldName, field.m_nSingleInheritanceOffset);
            return field.m_nSingleInheritanceOffset;
        }
    }

    CLogger::Log("[schemamgr] No field named '{}' has been found in binding '{}'.", fieldName, bindingName);
    return {};
}
