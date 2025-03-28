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
        Toggle();
    } else if (ImGui::IsKeyPressed(ImGuiKey_End, false)) {
        DeclareShutdown();
    }

    RenderWatermark();
    RenderUI();
}

void CMenu::HandleStates(bool mouseIsEnabled) {
    auto inputSystem = CInputSystem::Get();
    if (!inputSystem || !ImGui::GetCurrentContext()) {
        return;
    }

    const ImVec2 screenCenter = ImGui::GetIO().DisplaySize * 0.5f;
    switch (m_State) {
        case TOGGLE_CLOSE:
        case TOGGLE_OPEN:
            m_State = (m_State == TOGGLE_OPEN) ? OPEN : CLOSED;

            if (mouseIsEnabled) {
                sdl::SetWindowRelativeMouseMode(inputSystem->GetSDLWindow(), !IsOpen());
                sdl::SetWindowMouseGrab(inputSystem->GetSDLWindow(), !IsOpen());
                sdl::WarpMouseInWindow(nullptr, screenCenter.x, screenCenter.y);
            }

            break;
        case OPEN:
            if (mouseIsEnabled) {
                sdl::SetWindowRelativeMouseMode(inputSystem->GetSDLWindow(), false);
            }

            break;
        case TOGGLE_SHUTDOWN_AND_CLOSE_MENU:
            if (mouseIsEnabled) {
                sdl::SetWindowRelativeMouseMode(inputSystem->GetSDLWindow(), true);
            }

            m_State = TOGGLE_SHUTDOWN;

            break;
        case TOGGLE_SHUTDOWN:
            CInstance::Get().FreeLibrary();
            m_State = SHUTDOWN;

            break;
    }
}

void CMenu::DeclareShutdown() {
    SDK_LOG_PROLOGUE();

    m_State = IsOpen() ? TOGGLE_SHUTDOWN_AND_CLOSE_MENU : TOGGLE_SHUTDOWN;
}

void CMenu::Toggle() {
    if (m_State == OPEN || m_State == CLOSED) {
        m_State = static_cast<State>(m_State + 1);
    }
}

void CMenu::RenderWatermark() {
    auto drawList = CRenderer::GetBackgroundDrawList();

    char watermarkText[128];
    snprintf(watermarkText, IM_ARRAYSIZE(watermarkText), "cs2-sdk v2 [%d]\nFPS: %d\n\n%s %s\nhttps://github.com/bruhmoment21/cs2-sdk",
             CEngineClient::Get()->GetEngineBuildNumber(), static_cast<int>(ImGui::GetIO().Framerate), __DATE__, __TIME__);

    drawList->AddText({17, 9}, IM_COL32(0, 0, 0, 255), watermarkText);
    drawList->AddText({16, 8}, IM_COL32(27, 227, 200, 255), watermarkText);
}

void CMenu::RenderUI() {
    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    if (!IsOpen()) {
        return;
    }

    IO.ConfigFlags = ImGuiConfigFlags_None;

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

        if (ImGui::Button("Unload", {-FLT_MIN, 0})) DeclareShutdown();
    }
    ImGui::End();
}
