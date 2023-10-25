#include "pch.hpp"

#include <cache/entities/hen.hpp>

#include <vars/vars.hpp>

bool CCachedHen::CanDrawESP() { return Get() && g_Vars.m_ChickenESP; }
