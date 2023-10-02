#pragma once

class CGameHooks {
   public:
    static CGameHooks& Get() {
        static CGameHooks inst;
        return inst;
    }

    void Initialize();
};
