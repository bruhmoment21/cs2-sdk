#pragma once

class CRenderHooks {
   public:
    static CRenderHooks& Get() {
        static CRenderHooks inst;
        return inst;
    }

    void Initialize();
    bool RunningVulkan();
};
