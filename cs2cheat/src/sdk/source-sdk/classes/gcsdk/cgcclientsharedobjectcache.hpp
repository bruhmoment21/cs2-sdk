#pragma once

class CGCClientSharedObjectTypeCache;

class CGCClientSharedObjectCache {
   public:
    CGCClientSharedObjectTypeCache* CreateBaseTypeCache(int nClassID);
};
