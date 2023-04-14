#pragma once

class CGameEvent;
class CEconItem;

namespace skin_changer {
    void FrameStageNotify(int frameStage);
    void PreFireEvent(CGameEvent* pEvent);

    void AddEconItemToList(CEconItem* pItem);
    void Shutdown();
}  // namespace skin_changer
