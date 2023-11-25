#include <algorithm>
#include <vector>

#include "menu.hpp"

#include "../../sdk/interfaces/interfaces.hpp"
#include "../../utils/utils.hpp"

#include "../skins/skin_changer.hpp"
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

    RenderInventoryWindow();

    ImGui::SetNextWindowPos(IO.DisplaySize / 2.f, ImGuiCond_Once, {0.5f, 0.5f});
    ImGui::Begin("cs2sdk menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SeparatorText("Player ESP");
    ImGui::Checkbox("Players box", &esp::bBoxes);
    ImGui::Checkbox("Players name", &esp::bName);
    ImGui::Checkbox("Players healthbar", &esp::bHealthbar);
    ImGui::Checkbox("Active weapon name", &esp::bActiveWeaponName);
    ImGui::Checkbox("Ignore teammates", &esp::bIgnoreTeammates);
    ImGui::Checkbox("Ignore enemies", &esp::bIgnoreEnemies);
    ImGui::Checkbox("Ignore yourself", &esp::bIgnoreSelf);

    ImGui::SeparatorText("Weapon ESP");
    ImGui::Checkbox("Weapons box", &esp::bDroppedWeaponBoxes);
    ImGui::Checkbox("Weapons name", &esp::bDroppedWeaponName);

    ImGui::TextUnformatted("Maximum weapon visibility range:");
    ImGui::SetNextItemWidth(windowWidth);
    ImGui::SliderFloat("##sliderWeapon", &esp::fWeaponMaxDistance, 0.f, 150.f,
                       "%.2f meters");

    ImGui::SeparatorText("Other ESP");
    ImGui::Checkbox("Chickens box", &esp::bChickenBox);

    ImGui::SeparatorText("Other settings");
    ImGui::SetNextItemWidth(windowWidth);
    ImGui::InputTextWithHint("##l0", "Class", classInputText,
                             IM_ARRAYSIZE(classInputText));
    if (ImGui::Button("Print class layout in game console", {windowWidth, 0}))
        memory::schema_detailed_class_layout(nullptr, classInputText);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Type 'C_BaseEntity' to understand what this does.");

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
        memory::fnSDL_WarpMouseInWindow(nullptr, screenCenter.x,
                                        screenCenter.y);
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

            const bool isWeapon = pItem->IsWeapon();
            const bool isKnife = pItem->IsKnife(true);
            const bool isGloves = pItem->IsGlove(true);

            if (!isWeapon && !isKnife && !isGloves) continue;

            // Some items don't have names.
            const char* itemBaseName = pItem->m_pszItemBaseName;
            if (!itemBaseName || itemBaseName[0] == '\0') continue;

            const uint16_t defIdx = pItem->m_nDefIndex;

            DumpedItem_t dumpedItem;
            dumpedItem.m_name = interfaces::pLocalize->FindSafe(itemBaseName);
            dumpedItem.m_defIdx = defIdx;
            dumpedItem.m_rarity = pItem->m_nItemRarity;

            if (isKnife || isGloves) dumpedItem.m_unusualItem = true;

            // Add vanilla knives
            if (isKnife) {
                dumpedItem.m_dumpedSkins.emplace_back("Vanilla", 0, IR_ANCIENT);
            }

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

            // Sort skins by rarity.
            if (!dumpedItem.m_dumpedSkins.empty() && isWeapon) {
                std::sort(dumpedItem.m_dumpedSkins.begin(),
                          dumpedItem.m_dumpedSkins.end(),
                          [](const DumpedSkin_t& a, const DumpedSkin_t& b) {
                              return a.m_rarity > b.m_rarity;
                          });
            }

            vecDumpedItems.emplace_back(dumpedItem);
        }
    }

    if (!vecDumpedItems.empty()) {
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
                            uint32_t(pInventory->GetOwner().m_id);
                        pItem->m_unDefIndex = item.m_defIdx;
                        if (item.m_unusualItem) pItem->m_nQuality = IQ_UNUSUAL;
                        pItem->m_nRarity =
                            std::clamp(item.m_rarity + skin.m_rarity - 1, 0,
                                       (skin.m_rarity == 7) ? 7 : 6);

                        pItem->SetPaintKit((float)skin.m_ID);
                        pItem->SetPaintSeed(1.f);
                        if (pInventory->AddEconItem(pItem))
                            skin_changer::AddEconItemToList(pItem);
                    }
                }
            }
        }

        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Will cause lag on weaker computers.");

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

            char buttonLabel[128];
            snprintf(buttonLabel, 128, "Add every %s skin",
                     pSelectedItem->m_name.c_str());

            if (ImGui::Button(buttonLabel, {windowWidth, 0.f})) {
                for (const auto& skin : pSelectedItem->m_dumpedSkins) {
                    CEconItem* pItem = CEconItem::CreateInstance();
                    if (pItem) {
                        CCSPlayerInventory* pInventory =
                            CCSPlayerInventory::GetInstance();

                        auto highestIDs = pInventory->GetHighestIDs();

                        pItem->m_ulID = highestIDs.first + 1;
                        pItem->m_unInventory = highestIDs.second + 1;
                        pItem->m_unAccountID =
                            uint32_t(pInventory->GetOwner().m_id);
                        pItem->m_unDefIndex = pSelectedItem->m_defIdx;
                        if (pSelectedItem->m_unusualItem)
                            pItem->m_nQuality = IQ_UNUSUAL;
                        pItem->m_nRarity = std::clamp(
                            pSelectedItem->m_rarity + skin.m_rarity - 1, 0,
                            (skin.m_rarity == 7) ? 7 : 6);

                        pItem->SetPaintKit((float)skin.m_ID);
                        pItem->SetPaintSeed(1.f);
                        if (pInventory->AddEconItem(pItem))
                            skin_changer::AddEconItemToList(pItem);
                    }
                }
            }

            if (pSelectedItem->pSelectedSkin) {
                static float kitWear = 0.f;
                static int kitSeed = 1;
                static int gunKills = -1;
                static char gunName[32];

                bool vanillaSkin = pSelectedItem->pSelectedSkin->m_ID == 0;
                snprintf(
                    buttonLabel, 128, "Add %s%s%s",
                    pSelectedItem->m_name.c_str(), vanillaSkin ? "" : " | ",
                    vanillaSkin ? ""
                                : pSelectedItem->pSelectedSkin->m_name.c_str());

                if (ImGui::Button(buttonLabel, {windowWidth, 0.f})) {
                    CEconItem* pItem = CEconItem::CreateInstance();
                    if (pItem) {
                        CCSPlayerInventory* pInventory =
                            CCSPlayerInventory::GetInstance();

                        auto highestIDs = pInventory->GetHighestIDs();

                        pItem->m_ulID = highestIDs.first + 1;
                        pItem->m_unInventory = highestIDs.second + 1;
                        pItem->m_unAccountID =
                            uint32_t(pInventory->GetOwner().m_id);
                        pItem->m_unDefIndex = pSelectedItem->m_defIdx;

                        if (pSelectedItem->m_unusualItem)
                            pItem->m_nQuality = IQ_UNUSUAL;

                        // I don't know nor do care why the rarity is calculated
                        // like this. [Formula]
                        pItem->m_nRarity = std::clamp(
                            pSelectedItem->m_rarity +
                                pSelectedItem->pSelectedSkin->m_rarity - 1,
                            0,
                            (pSelectedItem->pSelectedSkin->m_rarity == 7) ? 7
                                                                          : 6);

                        pItem->SetPaintKit(
                            (float)pSelectedItem->pSelectedSkin->m_ID);
                        pItem->SetPaintSeed((float)kitSeed);
                        pItem->SetPaintWear(kitWear);

                        if (gunKills >= 0) {
                            pItem->SetStatTrak(gunKills);
                            pItem->SetStatTrakType(0);

                            // Applied automatically on knives.
                            if (pItem->m_nQuality != IQ_UNUSUAL)
                                pItem->m_nQuality = IQ_STRANGE;
                        }

                        if (gunName[0]) pItem->SetCustomName(gunName);

                        if (pInventory->AddEconItem(pItem))
                            skin_changer::AddEconItemToList(pItem);

                        kitWear = 0.f;
                        kitSeed = 1;
                        gunKills = -1;
                        memset(gunName, '\0', IM_ARRAYSIZE(gunName));
                    }
                }

                ImGui::Dummy({0, 8});
                ImGui::SeparatorText("Extra settings");

                ImGui::TextUnformatted("Wear Rating");
                ImGui::SetNextItemWidth(windowWidth);
                ImGui::SliderFloat("##slider1", &kitWear, 0.f, 1.f, "%.9f",
                                   ImGuiSliderFlags_Logarithmic);

                ImGui::TextUnformatted("Pattern Template");
                ImGui::SetNextItemWidth(windowWidth);
                ImGui::SliderInt("##slider2", &kitSeed, 1, 1000);

                ImGui::TextUnformatted("StatTrak Count");
                ImGui::SetNextItemWidth(windowWidth);
                ImGui::SliderInt("##slider3", &gunKills, -1, INT_MAX / 2,
                                 gunKills == -1 ? "Not StatTrak" : "%d",
                                 ImGuiSliderFlags_Logarithmic);

                ImGui::TextUnformatted("Custom Name");
                ImGui::SetNextItemWidth(windowWidth);
                ImGui::InputTextWithHint("##input1", "Default", gunName,
                                         IM_ARRAYSIZE(gunName));
            }
        }
    }

    ImGui::End();
}
