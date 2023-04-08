#include <algorithm>
#include <vector>

#include "menu.hpp"

#include "../../sdk/interfaces/interfaces.hpp"
#include "../../utils/utils.hpp"

#include "../esp/esp.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

static bool g_bMenuIsOpen;

static void RenderInventoryWindow();

void menu::Render() {
    static constexpr float windowWidth = 230.f;
    static char classInputText[64];

    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) Toggle(!IsOpen());
    if (!IsOpen()) return;

    IO.ConfigFlags = ImGuiConfigFlags_None;

    memory::fnSDL_SetRelativeMouseMode(false);
    memory::fnSDL_SetWindowGrab(interfaces::pInputSystem->GetSDLWindow(),
                                false);

    RenderInventoryWindow();

    ImGui::SetNextWindowPos(IO.DisplaySize / 2.f, ImGuiCond_Once, {0.5f, 0.5f});
    ImGui::Begin("cs2sdk menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Box", &esp::bBoxEsp);
    ImGui::Checkbox("Name", &esp::bNameEsp);
    ImGui::Checkbox("Healthbar", &esp::bHealthbar);
    ImGui::Checkbox("Ignore teammates", &esp::bIgnoreTeammates);
    ImGui::SetNextItemWidth(windowWidth);
    ImGui::InputTextWithHint("##l0", "Class", classInputText,
                             IM_ARRAYSIZE(classInputText));
    if (ImGui::Button("Print class layout", {windowWidth, 0}))
        memory::schema_detailed_class_layout(nullptr, classInputText);

    if (ImGui::Button("Unload", {windowWidth, 0})) utils::UnloadLibrary();
    ImGui::End();
}

void menu::Toggle(bool bState) {
    if (!ImGui::GetCurrentContext() || !interfaces::pInputSystem) return;

    g_bMenuIsOpen = bState;
    if (interfaces::pInputSystem->IsRelativeMouseMode()) {
        const ImVec2 screenCenter = ImGui::GetIO().DisplaySize / 2.f;

        memory::fnSDL_SetRelativeMouseMode(!g_bMenuIsOpen);
        memory::fnSDL_SetWindowGrab(interfaces::pInputSystem->GetSDLWindow(),
                                    !g_bMenuIsOpen);
        memory::fnSDL_WarpMouseInWindow(nullptr, (int)screenCenter.x,
                                        (int)screenCenter.y);
    }
}

bool menu::IsOpen() { return g_bMenuIsOpen; }

static void RenderInventoryWindow() {
    static constexpr float windowWidth = 540.f;

    struct DumpedSkin_t {
        std::string m_name = "";
        int m_ID = 0;
        int m_rarity = 0;
    };
    struct DumpedItem_t {
        std::string m_name = "";
        uint16_t m_defIdx = 0;
        int m_rarity = 0;
        bool m_unusualItem = false;
        std::vector<DumpedSkin_t> m_dumpedSkins{};
        DumpedSkin_t* pSelectedSkin = nullptr;
    };
    static std::vector<DumpedItem_t> vecDumpedItems;
    static DumpedItem_t* pSelectedItem = nullptr;

    CEconItemSchema* pItemSchema =
        interfaces::pClient->GetEconItemSystem()->GetEconItemSchema();

    ImGui::Begin("cs2sdk item dumper", nullptr,
                 ImGuiWindowFlags_AlwaysAutoResize);
    if (vecDumpedItems.empty() &&
        ImGui::Button("Dump items", {windowWidth, 0})) {
        const CUtlMap<int, CEconItemDefinition*>& vecItems =
            pItemSchema->GetSortedItemDefinitionMap();
        const CUtlMap<int, CPaintKit*>& vecPaintKits =
            pItemSchema->GetPaintKits();
        const CUtlMap<uint64_t, AlternateIconData_t>& vecAlternateIcons =
            pItemSchema->GetAlternateIconsMap();

        for (const auto& it : vecItems) {
            CEconItemDefinition* pItem = it.m_value;
            if (!pItem) continue;

            uint16_t defIdx = pItem->GetDefinitionIndex();

            constexpr auto CSGO_Type_Knife =
                hash_32_fnv1a_const("#CSGO_Type_Knife");
            constexpr auto Type_Hands = hash_32_fnv1a_const("#Type_Hands");

            const bool isWeapon = pItem->GetStickersSupportedCount() >= 4;
            const bool isKnife =
                hash_32_fnv1a_const(pItem->m_pszItemTypeName) ==
                    CSGO_Type_Knife &&
                defIdx >= 500;
            const bool isGloves =
                hash_32_fnv1a_const(pItem->m_pszItemTypeName) == Type_Hands &&
                defIdx != 5028 && defIdx != 5029;
            if (!isWeapon && !isKnife && !isGloves) continue;

            // Some items don't have names.
            const char* itemBaseName = pItem->m_pszItemBaseName;
            if (!itemBaseName || itemBaseName[0] == '\0') continue;

            DumpedItem_t dumpedItem;
            dumpedItem.m_name = interfaces::pLocalize->FindSafe(itemBaseName);
            dumpedItem.m_defIdx = defIdx;
            dumpedItem.m_rarity = pItem->m_nItemRarity;

            if (isKnife || isGloves) dumpedItem.m_unusualItem = true;

            // We filter skins by guns.
            for (const auto& it : vecPaintKits) {
                CPaintKit* pPaintKit = it.m_value;
                if (!pPaintKit || pPaintKit->nID == 0 || pPaintKit->nID == 9001)
                    continue;

                const uint64_t skinKey =
                    Helper_GetAlternateIconKeyForWeaponPaintWearItem(
                        defIdx, pPaintKit->nID, 0);
                if (vecAlternateIcons.FindByKey(skinKey).has_value()) {
                    DumpedSkin_t dumpedSkin;
                    dumpedSkin.m_name = interfaces::pLocalize->FindSafe(
                        pPaintKit->sDescriptionTag);
                    dumpedSkin.m_ID = pPaintKit->nID;
                    dumpedSkin.m_rarity = pPaintKit->nRarity;
                    dumpedItem.m_dumpedSkins.emplace_back(dumpedSkin);
                }
            }

            vecDumpedItems.emplace_back(dumpedItem);
        }
    }

    if (!vecDumpedItems.empty()) {
        static ImGuiTextFilter itemFilter;
        itemFilter.Draw("##filter", windowWidth);

        if (ImGui::BeginListBox("##items", {windowWidth, 140.f})) {
            for (auto& item : vecDumpedItems) {
                if (!itemFilter.PassFilter(item.m_name.c_str())) continue;

                ImGui::PushID(&item);
                if (ImGui::Selectable(item.m_name.c_str(),
                                      pSelectedItem == &item)) {
                    if (pSelectedItem == &item)
                        pSelectedItem = nullptr;
                    else
                        pSelectedItem = &item;
                }
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }

        if (pSelectedItem) {
            if (!pSelectedItem->m_dumpedSkins.empty()) {
                static ImGuiTextFilter skinFilter;
                skinFilter.Draw("##filter2", windowWidth);

                if (ImGui::BeginListBox("##skins", {windowWidth, 140.f})) {
                    for (auto& skin : pSelectedItem->m_dumpedSkins) {
                        if (!skinFilter.PassFilter(skin.m_name.c_str()))
                            continue;

                        ImGui::PushID(&skin);
                        if (ImGui::Selectable(
                                skin.m_name.c_str(),
                                pSelectedItem->pSelectedSkin == &skin)) {
                            if (pSelectedItem->pSelectedSkin == &skin)
                                pSelectedItem->pSelectedSkin = nullptr;
                            else
                                pSelectedItem->pSelectedSkin = &skin;
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndListBox();
                }
            }

            if (pSelectedItem->pSelectedSkin &&
                ImGui::Button("Add selected item", {windowWidth, 0.f})) {
                CEconItem* pItem = CEconItem::CreateInstance();
                if (pItem) {
                    CCSPlayerInventory* pInventory =
                        CCSPlayerInventory::GetInstance();

                    auto highestIDs = pInventory->GetHighestIDs();

                    pItem->m_ulID = highestIDs.first + 1;
                    pItem->m_unInventory = highestIDs.second + 1;
                    pItem->m_unAccountID =
                        uint32_t(pInventory->GetOwnerID().m_id);
                    pItem->m_unDefIndex = pSelectedItem->m_defIdx;

                    if (pSelectedItem->m_unusualItem)
                        pItem->m_nQuality = IQ_UNUSUAL;

                    // I don't know nor do care why the rarity is calculated
                    // like this. [Formula]
                    pItem->m_nRarity = std::clamp(
                        pSelectedItem->m_rarity +
                            pSelectedItem->pSelectedSkin->m_rarity - 1,
                        0,
                        (pSelectedItem->pSelectedSkin->m_rarity == 7) ? 7 : 6);

                    pItem->SetPaintKit(pSelectedItem->pSelectedSkin->m_ID);
                    pInventory->AddEconItem(pItem, true, false, true);
                }
            }
        }

        if (ImGui::Button("Add all items", {windowWidth, 0.f})) {
            for (const auto& item : vecDumpedItems) {
                for (const auto& skin : item.m_dumpedSkins) {
                    CEconItem* pItem = CEconItem::CreateInstance();
                    if (pItem) {
                        CCSPlayerInventory* pInventory =
                            CCSPlayerInventory::GetInstance();

                        auto highestIDs = pInventory->GetHighestIDs();

                        pItem->m_ulID = highestIDs.first + 1;
                        pItem->m_unInventory = highestIDs.second + 1;
                        pItem->m_unAccountID =
                            uint32_t(pInventory->GetOwnerID().m_id);
                        pItem->m_unDefIndex = item.m_defIdx;
                        if (item.m_unusualItem) pItem->m_nQuality = IQ_UNUSUAL;
                        pItem->m_nRarity =
                            std::clamp(item.m_rarity + skin.m_rarity - 1, 0,
                                       (skin.m_rarity == 7) ? 7 : 6);

                        pItem->SetPaintKit(skin.m_ID);
                        pInventory->AddEconItem(pItem, false, false, false);
                    }
                }
            }
        }
    }

    ImGui::End();
}
