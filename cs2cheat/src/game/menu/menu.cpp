#include "menu.hpp"

#include "../../sdk/interfaces/interfaces.hpp"
#include "../../utils/utils.hpp"

#include "../esp/esp.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

static bool g_bMenuIsOpen;
static constexpr float g_fMaxWidth = 210.f;

void menu::Render() {
    static char classInputText[64];

    if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) Toggle(!IsOpen());
    if (!IsOpen()) return;

    memory::fnSDL_SetRelativeMouseMode(false);
    memory::fnSDL_SetWindowGrab(interfaces::pInputSystem->GetSDLWindow(),
                                false);

    ImGuiIO& IO = ImGui::GetIO();
    ImGui::SetNextWindowPos(IO.DisplaySize / 2.f, ImGuiCond_Once, {0.5f, 0.5f});
    ImGui::Begin("cs2sdk menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Box", &esp::bBoxEsp);
    ImGui::Checkbox("Name", &esp::bNameEsp);
    ImGui::Checkbox("Healthbar", &esp::bHealthbar);
    ImGui::Checkbox("Ignore teammates", &esp::bIgnoreTeammates);
    ImGui::SetNextItemWidth(g_fMaxWidth);
    ImGui::InputTextWithHint("##l0", "Class", classInputText,
                             IM_ARRAYSIZE(classInputText));
    if (ImGui::Button("Print class layout", {g_fMaxWidth, 0}))
        memory::schema_detailed_class_layout(nullptr, classInputText);

    if (ImGui::Button("Add Karambit into inventory", {g_fMaxWidth, 0})) {
        CEconItem* pItem = CEconItem::CreateInstance();
        if (pItem) {
            pItem->m_unDefIndex = 507;
            CCSPlayerInventory::GetInstance()->AddEconItem(pItem, true, false,
                                                           true);
        }
    }

    if (ImGui::Button("Unload", {g_fMaxWidth, 0})) utils::UnloadLibrary();
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
