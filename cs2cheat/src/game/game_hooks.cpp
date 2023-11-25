#include "../sdk/interfaces/interfaces.hpp"
#include "../api/hook/hook.hpp"

#include "skins/skin_changer.hpp"
#include "menu/menu.hpp"
#include "esp/esp.hpp"

static CHook<bool __fastcall(void*)> g_mouseInputEnabled;
static bool __fastcall hkMouseInputEnabled(void* rcx) {
    if (menu::IsOpen()) return false;
    return g_mouseInputEnabled(rcx);
}

static CHook<void __fastcall(void*, int)> g_frameStageNotify;
static void __fastcall hkFrameStageNotify(void* rcx, int frameStage) {
    skin_changer::OnFrameStageNotify(frameStage);
    return g_frameStageNotify(rcx, frameStage);
}

static CHook<void* __fastcall(void*, CEntityInstance*, CHandle)> g_onAddEntity;
static void* __fastcall hkOnAddEntity(void* rcx, CEntityInstance* pInstance,
                                      CHandle hHandle) {
    esp::OnAddEntity(pInstance, hHandle);
    return g_onAddEntity(rcx, pInstance, hHandle);
}

static CHook<void* __fastcall(void*, CEntityInstance*, CHandle)>
    g_onRemoveEntity;
static void* __fastcall hkOnRemoveEntity(void* rcx, CEntityInstance* pInstance,
                                         CHandle hHandle) {
    esp::OnRemoveEntity(pInstance, hHandle);
    return g_onRemoveEntity(rcx, pInstance, hHandle);
}

static CHook<void __fastcall(void*, void*, VMatrix*, VMatrix*, VMatrix*,
                             VMatrix*)>
    g_getMatricesForView;
static void __fastcall hkGetMatricesForView(void* rcx, void* view,
                                            VMatrix* pWorldToView,
                                            VMatrix* pViewToProjection,
                                            VMatrix* pWorldToProjection,
                                            VMatrix* pWorldToPixels) {
    g_getMatricesForView(rcx, view, pWorldToView, pViewToProjection,
                         pWorldToProjection, pWorldToPixels);

    math::UpdateViewMatrix(pWorldToProjection);
    esp::CalculateBoundingBoxes();
}

static CHook<bool __fastcall(void*, CGameEvent*, bool)> g_fireEventClientSide;
static bool __fastcall hkFireEventClientSide(void* rcx, CGameEvent* event,
                                             bool bServerOnly) {
    skin_changer::OnPreFireEvent(event);
    return g_fireEventClientSide(rcx, event, bServerOnly);
}

static CHook<bool __fastcall(void*, int, int, uint64_t)> g_equipItemInLoadout;
static bool __fastcall hkEquipItemInLoadout(void* rcx, int iTeam, int iSlot,
                                            uint64_t iItemID) {
    skin_changer::OnEquipItemInLoadout(iTeam, iSlot, iItemID);
    return g_equipItemInLoadout(rcx, iTeam, iSlot, iItemID);
}

static CHook<void* __fastcall(C_BaseModelEntity*, const char*)> g_setModel;
static void* __fastcall hkSetModel(C_BaseModelEntity* rcx, const char* model) {
    skin_changer::OnSetModel(rcx, model);
    return g_setModel(rcx, model);
}

void CS2_HookGameFunctions() {
    g_mouseInputEnabled.Hook(memory::fnMouseInputEnabled,
                             HOOK_FUNCTION(hkMouseInputEnabled));
    g_frameStageNotify.HookVirtual(interfaces::pClient, 33,
                                   HOOK_FUNCTION(hkFrameStageNotify));
    g_onAddEntity.HookVirtual(CGameEntitySystem::GetInstance(), 14,
                              HOOK_FUNCTION(hkOnAddEntity));
    g_onRemoveEntity.HookVirtual(CGameEntitySystem::GetInstance(), 15,
                                 HOOK_FUNCTION(hkOnRemoveEntity));
    g_getMatricesForView.Hook(memory::fnGetMatricesForView,
                              HOOK_FUNCTION(hkGetMatricesForView));
    g_fireEventClientSide.Hook(memory::fnFireEventClientSide,
                               HOOK_FUNCTION(hkFireEventClientSide));
    g_equipItemInLoadout.HookVirtual(CCSInventoryManager::GetInstance(), 52,
                                     HOOK_FUNCTION(hkEquipItemInLoadout));
    g_setModel.Hook(memory::fnSetModel, HOOK_FUNCTION(hkSetModel));

    esp::CacheCurrentEntities();
}

void CS2_UnhookGameFunctions() { skin_changer::Shutdown(); }
