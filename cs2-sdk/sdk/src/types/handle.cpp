#include "pch.hpp"

#include <types/handle.hpp>

#include <interfaces/gameentitysystem.hpp>

#include <bindings/baseentity.hpp>

void CBaseHandle::Init(int entry, int serialNumber) { m_Index = entry | (serialNumber << NUM_SERIAL_NUM_SHIFT_BITS); }

bool CBaseHandle::IsValid() const { return m_Index != INVALID_EHANDLE_INDEX; }

int CBaseHandle::GetEntryIndex() const { return IsValid() ? m_Index & ENT_ENTRY_MASK : ENT_ENTRY_MASK; }

int CBaseHandle::GetSerialNumber() const { return m_Index >> NUM_SERIAL_NUM_SHIFT_BITS; }

int CBaseHandle::ToInt() const { return static_cast<int>(m_Index); }

bool CBaseHandle::operator==(CBaseHandle rhs) const { return m_Index == rhs.m_Index; }

C_BaseEntity* CBaseHandle::Get() const {
    if (!IsValid()) return nullptr;

    C_BaseEntity* ent = CGameEntitySystem::GetBaseEntity(GetEntryIndex());
    if (!ent || ent->GetRefEHandle() != *this) return nullptr;

    return ent;
}
