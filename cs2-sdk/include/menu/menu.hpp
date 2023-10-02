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

    bool m_Open;

    static constexpr float m_WindowWidth = 210.f;
};
