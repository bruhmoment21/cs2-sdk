#pragma once

template <typename T>
class CNetworkUtlVectorBase {
   public:
    int m_size;
    char pad0[0x4];  // no idea
    T* m_data;
};
