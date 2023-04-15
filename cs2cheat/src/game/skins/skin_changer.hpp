#pragma once

class CEconDefaultEquippedDefinitionInstanceClient;
class CGameEvent;
class CEconItem;

namespace skin_changer {
    void OnFrameStageNotify(int frameStage);
    void OnPreFireEvent(CGameEvent* pEvent);
    void OnSoUpdated(CEconDefaultEquippedDefinitionInstanceClient* pObject);
    void OnEquipItemInLoadout(int team, int slot, uint64_t itemID);

    void AddEconItemToList(CEconItem* pItem);
    void Shutdown();
}  // namespace skin_changer
