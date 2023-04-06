#pragma once

#include "../../schema/schema.hpp"

#include "../types/chandle.hpp"

class CCSPlayer_ViewModelServices {
   public:
    PSCHEMA_FIELD(m_hViewModel, "CCSPlayer_ViewModelServices", "m_hViewModel",
                  CHandle);
};
