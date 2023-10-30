#pragma once

#include <cache/entities/base.hpp>

// Has been named "Captive" to possibly avoid future naming conflicts.

class C_Hostage;

class CCachedCaptive : public CCachedBaseEntity {
   public:
    Type GetType() const override { return Type::HOSTAGE; }

    auto Get() const { return CCachedBaseEntity::Get<C_Hostage>(); }

    bool CanDoESP() override;
    void DrawESP() override;
};
