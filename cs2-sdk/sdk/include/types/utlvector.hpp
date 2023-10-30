#pragma once

template <typename T>
class CUtlVector {
   public:
    auto At(int i) const { return m_Data[i]; }
    auto AtPtr(int i) const { return m_Data + i; }

    // C++ STL Iterators
    auto begin() const { return m_Data; }
    auto end() const { return m_Data + m_Size; }

    int m_Size;
    char pad0[0x4];
    T* m_Data;
    char pad1[0x8];
};
