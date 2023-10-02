#pragma once

#include <cache/entities/base.hpp>

// Has been named "Hen" to possibly avoid future conflicts.

class CCachedHen : public CCachedBaseEntity {
   public:
    CCachedHen(CBaseHandle handle);

    bool CanDrawESP() override;
};
