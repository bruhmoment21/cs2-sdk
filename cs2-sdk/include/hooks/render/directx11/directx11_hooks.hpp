#pragma once

class CDirectX11Hooks {
   public:
    static CDirectX11Hooks& Get() {
        static CDirectX11Hooks inst;
        return inst;
    }

    void Initialize();

   private:
    ~CDirectX11Hooks();
};
