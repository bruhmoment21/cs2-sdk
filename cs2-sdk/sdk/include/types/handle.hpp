#pragma once

constexpr auto INVALID_EHANDLE_INDEX = 0xffffffff;
constexpr auto ENT_ENTRY_MASK = 0x7fff;
constexpr auto NUM_SERIAL_NUM_SHIFT_BITS = 15;

class C_BaseEntity;

class CBaseHandle {
   public:
    CBaseHandle() : m_Index(INVALID_EHANDLE_INDEX) {}
    CBaseHandle(uint32_t value) : m_Index(value) {}
    CBaseHandle(int entry, int serialNumber) { Init(entry, serialNumber); }

    void Init(int entry, int serialNumber);

    bool IsValid() const;

    int GetEntryIndex() const;
    int GetSerialNumber() const;

    int ToInt() const;
    bool operator==(CBaseHandle rhs) const;

    C_BaseEntity* Get() const;

   protected:
    uint32_t m_Index;
};

template <typename T>
class CHandle : public CBaseHandle {
   public:
    auto Get() const { return reinterpret_cast<T*>(CBaseHandle::Get()); }
};
