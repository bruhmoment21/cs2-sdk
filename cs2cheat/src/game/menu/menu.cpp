#include "menu.hpp"

#include "../../sdk/interfaces/interfaces.hpp"
#include "../../utils/utils.hpp"

#include "../esp/esp.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

static bool g_bMenuIsOpen;

void menu::Render() {
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
    if (ImGui::Button("Unload", {128, 0})) utils::UnloadLibrary();
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
