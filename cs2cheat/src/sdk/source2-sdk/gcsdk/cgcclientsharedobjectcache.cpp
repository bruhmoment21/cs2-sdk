#include "cgcclientsharedobjectcache.hpp"

#include "../../memory/memory.hpp"

CGCClientSharedObjectTypeCache* CGCClientSharedObjectCache::CreateBaseTypeCache(
    int nClassID) {
    if (!memory::fnCreateBaseTypeCache) return nullptr;
    return memory::fnCreateBaseTypeCache(this, nClassID);
}
