#include "pch.hpp"

#include <renderer/renderer.hpp>
#include <logger/logger.hpp>

#include <hacks/esp/esp.hpp>

#include <menu/menu.hpp>

#include <imgui/imgui.h>

void CRenderer::Initialize() { SDK_LOG_PROLOGUE(); }

void CRenderer::NewFrame() {
    m_BackgroundDrawList = ImGui::GetBackgroundDrawList();

    CMenu::Get().Render();
    CESP::Get().Render();
}
