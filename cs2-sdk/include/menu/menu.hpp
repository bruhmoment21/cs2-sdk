#pragma once

class CMenu {
   public:
    static CMenu& Get() {
        static CMenu inst;
        return inst;
    }

    auto IsOpen() const { return m_State == OPEN || m_State == TOGGLE_CLOSE; }

    void Render();
    void HandleStates(bool mouseIsEnabled);

   private:
    void DeclareShutdown();

    void Toggle();

    void RenderWatermark();
    void RenderUI();
    void RenderMainMenu();

    enum State { CLOSED = 0, TOGGLE_OPEN, OPEN, TOGGLE_CLOSE, TOGGLE_SHUTDOWN, TOGGLE_SHUTDOWN_AND_CLOSE_MENU, SHUTDOWN };

    State m_State;
};
