#include "../../utils/utils.hpp"

#include "../menu/menu.hpp"
#include "../esp/esp.hpp"

#include "render.hpp"

#include <imgui/imgui.h>

void render::Initialize() {
    // Initialize fonts/colors/style here.
}

void render::NewFrame() {
    if (ImGui::IsKeyPressed(ImGuiKey_End, false)) utils::UnloadLibrary();

    esp::Render();
    menu::Render();
}

void render::Shutdown() { menu::Toggle(false); }
