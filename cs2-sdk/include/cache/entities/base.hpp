#pragma once

#include <types/handle.hpp>

#include <math/types/bbox.hpp>

class CCachedBaseEntity {
   public:
    enum Type { GENERIC = 0, PLAYER, GUN, HEN };

    CCachedBaseEntity() { m_BoxColor = IM_COL32(255, 255, 255, 255); }
    CCachedBaseEntity(const CCachedBaseEntity&) = delete;
    CCachedBaseEntity& operator=(const CCachedBaseEntity&) = delete;

    // Gets rid of the need to dynamic_cast.
    virtual Type GetType() const { return Type::GENERIC; }

    template <typename T = C_BaseEntity>
    auto Get() {
        return CHandle<T>(m_Handle).Get();
    }
    auto Set(CBaseHandle handle) { m_Handle = handle; }

    virtual bool CanDrawESP();
    virtual void ResetESP();
    virtual void RenderESP();
    virtual void UpdateESP();

   protected:
    CBaseHandle m_Handle;

    BBox_t m_BBox;
    unsigned int m_BoxColor, m_BoxOutlineColor = IM_COL32(0, 0, 0, 255);
};
