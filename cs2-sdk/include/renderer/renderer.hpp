#pragma once

struct ImDrawList;

class CRenderer {
   public:
    static CRenderer& Get() {
        static CRenderer inst;
        return inst;
    }

    static auto GetBackgroundDrawList() { return Get().m_BackgroundDrawList; }

    void Initialize();
    void NewFrame();

   private:
    ImDrawList* m_BackgroundDrawList;
};
