#pragma once

class CInstance {
   public:
    static CInstance& Get() {
        static CInstance inst;
        return inst;
    }

    void Initialize();
    void Shutdown();

    bool AppFullyLoaded();

    // Not the same as 'Shutdown'. Call this when you want to unload.
    void FreeLibrary();
};
