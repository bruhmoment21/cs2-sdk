#pragma once

class CSDLHooks {
   public:
    static CSDLHooks& Get() {
        static CSDLHooks inst;
        return inst;
    }

    void Initialize();
};
