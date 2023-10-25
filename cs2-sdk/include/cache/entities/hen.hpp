#pragma once

#include <cache/entities/base.hpp>

// Has been named "Hen" to possibly avoid future naming conflicts.

class CCachedHen : public CCachedBaseEntity {
   public:
    CCachedHen() { m_BoxColor = IM_COL32(206, 66, 245, 255); }

    Type GetType() const override { return Type::HEN; }

    bool CanDrawESP() override;
};
