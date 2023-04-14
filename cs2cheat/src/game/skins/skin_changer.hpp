#pragma once

class CEconItem;

namespace skin_changer {
    void FrameStageNotify(int frameStage);
    void AddEconItemToList(CEconItem* pItem);
    void Shutdown();
}  // namespace skin_changer
