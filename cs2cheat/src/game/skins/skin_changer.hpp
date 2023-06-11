#pragma once

class CGameEvent;
class CEconItem;
class C_BaseModelEntity;

namespace skin_changer {
    void OnFrameStageNotify(int frameStage);
    void OnPreFireEvent(CGameEvent* pEvent);
    void OnEquipItemInLoadout(int team, int slot, uint64_t itemID);
    void OnSetModel(C_BaseModelEntity* pEntity, const char*& model);

    void AddEconItemToList(CEconItem* pItem);
    void Shutdown();
}  // namespace skin_changer
