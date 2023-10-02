#include "pch.hpp"

#include <cache/entities/hen.hpp>

#include <renderer/renderer.hpp>
#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

CCachedHen::CCachedHen(CBaseHandle handle) : CCachedBaseEntity(handle) { m_BoxColor = IM_COL32(206, 66, 245, 255); }

bool CCachedHen::CanDrawESP() { return Get() && g_Vars.m_ChickenESP; }
