#include "cgcclient.hpp"

#include "../../memory/memory.hpp"

CGCClientSharedObjectCache* CGCClient::FindSOCache(SOID_t ID,
                                                   bool bCreateIfMissing) {
    if (!memory::fnFindSOCache) return nullptr;
    return memory::fnFindSOCache(this, ID, bCreateIfMissing);
}
