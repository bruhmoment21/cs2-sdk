#pragma once

#include <optional>

// [!] PSA:
//
// Terrible implementation because CUtlMap should allow us to search items in
// O(log n) but I have implemented it more like a Vector due to lack of my
// understanding of CUtlMap.

template <typename K, typename V>
class CUtlMap {
   public:
    struct Node_t {
        char pad0[0x8];  // no idea
        char pad1[0x8];  // no idea
        K m_key;
        V m_value;
    };

    auto begin() const { return m_data; }
    auto end() const { return m_data + m_size; }

    std::optional<V> FindByKey(K key) const {
        for (int i = 0; i < m_size; ++i)
            if (m_data[i].m_key == key) return m_data[i].m_value;
        return {};
    }

    char pad0[0x8];  // no idea
    Node_t* m_data;
    char pad1[0x8];  // no idea
    char pad2[0x4];  // no idea
    int m_size;
    char pad3[0x8];  // no idea
};
