#include "../utils/utils.hpp"

#include "menu/menu.hpp"
#include "esp/esp.hpp"

#include <imgui/imgui.h>

void CS2_RenderUI() {
    if (ImGui::IsKeyPressed(ImGuiKey_End, false)) utils::UnloadLibrary();

    esp::Render();
    menu::Render();
}
