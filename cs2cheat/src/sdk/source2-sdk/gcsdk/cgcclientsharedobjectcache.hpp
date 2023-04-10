#pragma once

enum ESOCacheEvent {
    /// Dummy sentinel value
    eSOCacheEvent_None = 0,

    /// We received a our first update from the GC and are subscribed
    eSOCacheEvent_Subscribed = 1,

    /// We lost connection to GC or GC notified us that we are no longer
    /// subscribed. Objects stay in the cache, but we no longer receive updates
    eSOCacheEvent_Unsubscribed = 2,

    /// We received a full update from the GC on a cache for which we were
    /// already subscribed. This can happen if connectivity is lost, and then
    /// restored before we realized it was lost.
    eSOCacheEvent_Resubscribed = 3,

    /// We received an incremental update from the GC about specific object(s)
    /// being added, updated, or removed from the cache
    eSOCacheEvent_Incremental = 4,

    /// A lister was added to the cache
    /// @see CGCClientSharedObjectCache::AddListener
    eSOCacheEvent_ListenerAdded = 5,

    /// A lister was removed from the cache
    /// @see CGCClientSharedObjectCache::RemoveListener
    eSOCacheEvent_ListenerRemoved = 6,
};

class CGCClientSharedObjectTypeCache;

class CGCClientSharedObjectCache {
   public:
    CGCClientSharedObjectTypeCache* CreateBaseTypeCache(int nClassID);
};
