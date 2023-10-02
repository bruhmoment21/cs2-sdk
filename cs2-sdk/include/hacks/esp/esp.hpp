#pragma once

class CESP {
   public:
    static CESP& Get() {
        static CESP inst;
        return inst;
    }

    bool IsEnabled();

    void Render();
    void Update();
};
