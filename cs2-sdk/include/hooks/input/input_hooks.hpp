#pragma once

class CInputHooks {
   public:
    static CInputHooks& Get() {
        static CInputHooks inst;
        return inst;
    }

    void Initialize();
    void Shutdown();
};
