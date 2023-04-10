#pragma once

template <typename T>
class CNetworkUtlVectorBase {
   public:
    auto begin() const { return m_data; }
    auto end() const { return m_data + m_size; }

    int m_size;
    char pad0[0x4];  // no idea
    T* m_data;
};
