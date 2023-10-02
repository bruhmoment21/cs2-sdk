#pragma once

#include <cache/entities/base.hpp>

class CCachedWeapon : public CCachedBaseEntity {
   public:
    CCachedWeapon(CBaseHandle handle);

    bool CanDrawESP() override;
};
