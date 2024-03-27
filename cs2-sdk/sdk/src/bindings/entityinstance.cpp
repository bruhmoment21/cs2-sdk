#include "pch.hpp"

#include <bindings/entityinstance.hpp>

#include <virtual/virtual.hpp>

CSchemaClassInfo* CEntityInstance::Schema_DynamicBinding() {
    CSchemaClassInfo* binding = nullptr;

#ifdef _WIN32
    vt::CallMethod<void>(this, 36, &binding);
#elif __linux__
    binding = vt::CallMethod<CSchemaClassInfo*>(this, 37);
#endif

    return binding;
}

CBaseHandle CEntityInstance::GetRefEHandle() {
    CBaseHandle handle{};

    CEntityIdentity* identity = m_pEntity();
    if (!identity) return handle;

    CBaseHandle unkHandle = CPointer(identity).GetField<CBaseHandle>(0x10);
    handle.Init(unkHandle.GetEntryIndex(), unkHandle.GetSerialNumber() - (identity->m_flags() & 1));

    return handle;
}
