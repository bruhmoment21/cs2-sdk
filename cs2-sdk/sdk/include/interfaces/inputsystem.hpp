#pragma once

class CInputSystem {
   public:
    static CInputSystem* Get();

    bool IsRelativeMouseMode();
    void* GetSDLWindow();
};
