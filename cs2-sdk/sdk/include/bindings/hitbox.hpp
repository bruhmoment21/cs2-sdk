#pragma once

#include <types/utlvector.hpp>

class CHitBox {
   public:
    SCHEMA_EXTENDED(Vector, m_vMinBounds, CConstants::ANIMATIONSYSTEM_LIB, "CHitBox", "m_vMinBounds", 0);
    SCHEMA_EXTENDED(Vector, m_vMaxBounds, CConstants::ANIMATIONSYSTEM_LIB, "CHitBox", "m_vMaxBounds", 0);

   private:
    char pad[0x70];  // sizeof CHitBox
};

class CHitBoxSet {
   public:
    SCHEMA_EXTENDED(CUtlVector<CHitBox>, m_HitBoxes, CConstants::ANIMATIONSYSTEM_LIB, "CHitBoxSet", "m_HitBoxes", 0);
};
