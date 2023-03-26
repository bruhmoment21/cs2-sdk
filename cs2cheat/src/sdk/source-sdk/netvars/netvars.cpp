#include <unordered_map>
#include <mutex>

#include "netvars.hpp"

#include "../../interfaces/interfaces.hpp"

// [!] ADD THE CLASSES YOU NEED HERE.
// Format: {className, module}.
static constexpr std::pair<const char*, const char*> g_classes[]{
    {"CCSPlayerController", CLIENT_DLL},
    {"CBasePlayerController", CLIENT_DLL},
    {"CEntityIdentity", CLIENT_DLL},
    {"CEntityInstance", CLIENT_DLL},
};

using NetvarKeyValueMap_t = std::unordered_map<uint32_t, int16_t>;
using NetvarTableMap_t = std::unordered_map<uint32_t, NetvarKeyValueMap_t>;

static void InitNetvars(NetvarTableMap_t& tableMap) {
    for (auto [szClass, module] : g_classes) {
        uint32_t classHash = hash_32_fnv1a_const(szClass);

        CSchemaSystemTypeScope* pType =
            interfaces::pSchemaSystem->FindTypeScopeForModule(module);
        if (!pType) continue;

        SchemaClassInfoData_t* pClassInfo = pType->FindDeclaredClass(szClass);
        if (!pClassInfo) continue;

        short fieldsSize = pClassInfo->GetFieldsSize();
        SchemaClassFieldData_t* pFields = pClassInfo->GetFields();

        auto& keyValueMap = tableMap[classHash];
        keyValueMap.reserve(fieldsSize);

        for (int i = 0; i < fieldsSize; ++i) {
            SchemaClassFieldData_t& field = pFields[i];
            keyValueMap.emplace(hash_32_fnv1a_const(field.pName),
                                field.uOffset);
        }

        LOG("netvarTableMap[%s] has %llu fields.\n", szClass,
            keyValueMap.size());
    }
}

uintptr_t netvars::GetOffset(uint32_t uTableKey, uint32_t uPropKey) {
    static NetvarTableMap_t netvarTableMap;
    static std::once_flag flag;
    std::call_once(flag, []() { InitNetvars(netvarTableMap); });

    const auto& tableMapIt = netvarTableMap.find(uTableKey);
    if (tableMapIt == netvarTableMap.cend()) return 0;

    const NetvarKeyValueMap_t& tableMap = tableMapIt->second;
    if (tableMap.find(uPropKey) == tableMap.cend()) return 0;

    return tableMap.at(uPropKey);
}
