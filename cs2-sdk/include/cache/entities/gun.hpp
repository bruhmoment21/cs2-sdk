#pragma once

#include <cache/entities/base.hpp>

// Has been named "Gun" to possibly avoid future naming conflicts.
// CCachedGun can also be a knife/taser/etc.

class CCachedGun : public CCachedBaseEntity {
   public:
    Type GetType() const override { return Type::GUN; }

    bool CanDoESP() override;
    void DrawESP() override;
};
