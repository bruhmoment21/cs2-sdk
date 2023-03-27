#include <unordered_map>

#include "netvars.hpp"

#include "../../interfaces/interfaces.hpp"

using NetvarKeyValueMap_t = std::unordered_map<uint32_t, int16_t>;
using NetvarTableMap_t = std::unordered_map<uint32_t, NetvarKeyValueMap_t>;

static bool InitNetvarsForClass(NetvarTableMap_t& tableMap,
                                const char* className, uint32_t classKey) {
    // CS2TODO: Most classes are in CLIENT_DLL. Should add more sanity checks.
    CSchemaSystemTypeScope* pType =
        interfaces::pSchemaSystem->FindTypeScopeForModule(CLIENT_DLL);
    if (!pType) return false;

    SchemaClassInfoData_t* pClassInfo = pType->FindDeclaredClass(className);
    if (!pClassInfo) {
        tableMap.emplace(classKey, NetvarKeyValueMap_t{});

        LOG("InitNetvarsForClass(): Class '%s' was not found!\n", className);
        return false;
    }

    short fieldsSize = pClassInfo->GetFieldsSize();
    SchemaClassFieldData_t* pFields = pClassInfo->GetFields();

    auto& keyValueMap = tableMap[classKey];
    keyValueMap.reserve(fieldsSize);

    for (int i = 0; i < fieldsSize; ++i) {
        SchemaClassFieldData_t& field = pFields[i];
        keyValueMap.emplace(hash_32_fnv1a_const(field.pName), field.uOffset);
    }

    LOG("netvarTableMap[%s] has %llu fields.\n", className, keyValueMap.size());
    return true;
}

uintptr_t netvars::GetOffset(const char* className, uint32_t classKey,
                             const char* memberName, uint32_t memberKey) {
    static NetvarTableMap_t netvarTableMap;
    const auto& tableMapIt = netvarTableMap.find(classKey);
    if (tableMapIt == netvarTableMap.cend()) {
        InitNetvarsForClass(netvarTableMap, className, classKey);
        return GetOffset(className, classKey, memberName, memberKey);
    }

    const NetvarKeyValueMap_t& tableMap = tableMapIt->second;
    if (tableMap.find(memberKey) == tableMap.cend()) {
        LOG("netvars::GetOffset(): '%s' was not found in '%s'!\n", memberName,
            className);
        return 0;
    }

    return tableMap.at(memberKey);
}
