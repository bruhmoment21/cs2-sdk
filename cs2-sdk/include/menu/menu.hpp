#pragma once

class CMenu {
   public:
    static CMenu& Get() {
        static CMenu inst;
        return inst;
    }

    auto IsOpen() const { return m_Open; }

    void Render();
    void Shutdown();

   private:
    void Toggle(bool state);

    void RenderWatermark();
    void RenderUI();

    void RenderMainMenu();

    bool m_Open;
};
