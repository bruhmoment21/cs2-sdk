#pragma once

#include <types/handle.hpp>

#include <math/types/bbox.hpp>

class CCachedBaseEntity {
   public:
    CCachedBaseEntity(CBaseHandle handle);

    CCachedBaseEntity(const CCachedBaseEntity&) = delete;
    CCachedBaseEntity& operator=(const CCachedBaseEntity&) = delete;

    template <typename T = C_BaseEntity>
    auto Get() {
        return CHandle<T>(m_Handle).Get();
    }

    virtual bool CanDrawESP();
    virtual void ResetESP();
    virtual void RenderESP();
    virtual void UpdateESP();

   protected:
    CBaseHandle m_Handle;

    BBox_t m_BBox;
    unsigned int m_BoxColor, m_BoxOutlineColor;
};
