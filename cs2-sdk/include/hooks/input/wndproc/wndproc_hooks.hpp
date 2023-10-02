#pragma once

class CWndProcHooks {
   public:
    static CWndProcHooks& Get() {
        static CWndProcHooks inst;
        return inst;
    }

#ifdef _WIN32
    auto GetWindow() const { return m_Window; }
#endif

    void Initialize();
    void Shutdown();

   private:
#ifdef _WIN32
    HWND m_Window;
#endif
};
