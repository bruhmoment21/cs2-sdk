#pragma once

#include <cache/entities/base.hpp>

// Has been named "Hen" to possibly avoid future naming conflicts.

class CCachedHen : public CCachedBaseEntity {
   public:
    Type GetType() const override { return Type::HEN; }

    void DrawESP() override;
};
