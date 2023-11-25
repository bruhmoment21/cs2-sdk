#include "pch.hpp"

#include <menu/menu.hpp>
#include <logger/logger.hpp>

#include <vars/vars.hpp>

#include <instance/instance.hpp>
#include <renderer/renderer.hpp>

#include <interfaces/engineclient.hpp>
#include <interfaces/inputsystem.hpp>

#include <sdl/sdl.hpp>

#include <imgui/imgui.h>

void CMenu::Render() {
    if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) {
        Toggle(!IsOpen());
    } else if (ImGui::IsKeyPressed(ImGuiKey_End, false)) {
        return CInstance::Get().FreeLibrary();
    }

    RenderWatermark();
    RenderUI();
}

void CMenu::Shutdown() {
    SDK_LOG_PROLOGUE();

    Toggle(false);
}

void CMenu::Toggle(bool state) {
    if (!ImGui::GetCurrentContext()) {
        return;
    }

    m_Open = state;

    auto inputSystem = CInputSystem::Get();
    if (inputSystem && inputSystem->IsRelativeMouseMode()) {
        const ImVec2 screenCenter = ImGui::GetIO().DisplaySize * 0.5f;

        sdl::SetRelativeMouseMode(!m_Open);
        sdl::SetWindowGrab(inputSystem->GetSDLWindow(), !m_Open);
        sdl::WarpMouseInWindow(nullptr, screenCenter.x, screenCenter.y);
    }
}

void CMenu::RenderWatermark() {
    auto drawList = CRenderer::GetBackgroundDrawList();

    char framerate[128];
    snprintf(framerate, IM_ARRAYSIZE(framerate), "cs2-sdk v2 [%d]\nFPS: %d\n\n%s %s\nhttps://github.com/bruhmoment21/cs2-sdk",
             CEngineClient::Get()->GetEngineBuildNumber(), static_cast<int>(ImGui::GetIO().Framerate), __DATE__, __TIME__);

    drawList->AddText({17, 9}, IM_COL32(0, 0, 0, 255), framerate);
    drawList->AddText({16, 8}, IM_COL32(27, 227, 200, 255), framerate);
}

void CMenu::RenderUI() {
    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    if (!IsOpen()) {
        return;
    }

    IO.ConfigFlags = ImGuiConfigFlags_None;

    auto inputSystem = CInputSystem::Get();
    if (inputSystem) {
        sdl::SetRelativeMouseMode(false);
    }

    RenderMainMenu();
}

void CMenu::RenderMainMenu() {
    constexpr auto windowStartWidth = 256.f;

    const ImGuiIO& IO = ImGui::GetIO();

    ImGui::SetNextWindowSize({windowStartWidth, 0}, ImGuiCond_Once);
    ImGui::SetNextWindowPos(IO.DisplaySize * 0.5f, ImGuiCond_Once, {0.5f, 0.5f});
    ImGui::Begin("cs2-sdk | main menu", nullptr);
    {
        ImGui::Checkbox("Master switch", &g_Vars.m_EnableESP);
        ImGui::SeparatorText("Visuals");

        ImGui::Checkbox("Players box", &g_Vars.m_PlayerBoxes);
        ImGui::Checkbox("Players name", &g_Vars.m_PlayerNames);
        ImGui::Checkbox("Players healthbar", &g_Vars.m_PlayerHealthBar);

        ImGui::Checkbox("Weapons box", &g_Vars.m_WeaponBoxes);
        ImGui::Checkbox("Chickens box", &g_Vars.m_ChickenBoxes);
        ImGui::Checkbox("Hostages box", &g_Vars.m_HostageBoxes);
        ImGui::Checkbox("Others box", &g_Vars.m_OtherBoxes);

        ImGui::Checkbox("Three-dimensional boxes", &g_Vars.m_Use3DBoxes);

        if (ImGui::Button("Unload", {-FLT_MIN, 0})) CInstance::Get().FreeLibrary();
    }
    ImGui::End();
}
