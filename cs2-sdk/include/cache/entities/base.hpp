#pragma once

#include <types/handle.hpp>

#include <math/types/bbox.hpp>

class CCachedBaseEntity {
   public:
    enum class Type { GENERIC = 0, PLAYER, GUN, HEN, HOSTAGE };

    CCachedBaseEntity() {}
    CCachedBaseEntity(const CCachedBaseEntity&) = delete;
    CCachedBaseEntity& operator=(const CCachedBaseEntity&) = delete;

    // Gets rid of the need to dynamic_cast.
    virtual Type GetType() const { return Type::GENERIC; }

    template <typename T = C_BaseEntity>
    auto Get() const {
        return CHandle<T>(m_Handle).Get();
    }

    auto GetHandle() const { return m_Handle; }
    auto SetHandle(CBaseHandle handle) { m_Handle = handle; }

    auto GetIndex() const { return m_Handle.GetEntryIndex(); }
    auto GetSerial() const { return m_Handle.GetSerialNumber(); }

    // Used by ESP.
    virtual bool CanDoESP();
    virtual void DrawESP();
    virtual void InvalidateDrawInfo();
    virtual void CalculateDrawInfo();

   protected:
    void DrawBoundingBox(ImU32 color);

    CBaseHandle m_Handle;
    BBox_t m_BBox;
};
