#pragma once

#include <cstdint>

class CGCClientSharedObjectCache;

struct SOID_t {
    uint64_t m_id;
    uint32_t m_type;
    uint32_t m_padding;
};

class CGCClient {
   public:
    CGCClientSharedObjectCache* FindSOCache(SOID_t ID,
                                            bool bCreateIfMissing = true);
};
