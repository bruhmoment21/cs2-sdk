#include "pch.hpp"

#include <interfaces/schemasystem.hpp>
#include <logger/logger.hpp>

#include <constants/constants.hpp>
#include <memory/memory.hpp>

#include <virtual/virtual.hpp>

CSchemaSystem* CSchemaSystem::Get() {
    static const auto inst = CMemory::GetInterface(CConstants::SCHEMASYSTEM_LIB, "SchemaSystem_001");
    return inst.Get<CSchemaSystem*>();
}

CSchemaSystemTypeScope* CSchemaSystem::FindTypeScopeForModule(const char* name) {
    return vt::CallMethod<CSchemaSystemTypeScope*>(this, 13, name, nullptr);
}

CSchemaClassInfo* CSchemaSystemTypeScope::FindDeclaredClass(const char* name) {
    CSchemaClassInfo* binding = nullptr;

#if _WIN32
    vt::CallMethod<void>(this, 2, &binding, name);
#elif __linux__
    binding = vt::CallMethod<CSchemaClassInfo*>(this, 2, name);
#endif

    return binding;
}

bool CSchemaClassInfo::InheritsFrom(CSchemaClassInfo* classToInherit) {
    if (classToInherit == this && classToInherit) {
        return true;
    } else if (!this || !m_BaseClasses || !classToInherit) {
        return false;
    }

    for (int i = 0; i < m_nBaseClassesCount; ++i) {
        auto& baseClass = m_BaseClasses[i];
        if (baseClass.m_pClass->InheritsFrom(classToInherit)) {
            return true;
        }
    }

    return false;
}
