#include <thread>

#include "../../utils/utils.hpp"

#include "../menu/menu.hpp"
#include "../esp/esp.hpp"

#include "render.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_freetype.h>

void render::Initialize() {
    // Initialize fonts/colors/style here.
    ImGuiIO& IO = ImGui::GetIO();

    ImFontConfig cfg{};
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Bitmap;

    IO.Fonts->Clear();
    IO.Fonts->AddFontFromFileTTF("E:\\cozette_bitmap.ttf", 13.f, &cfg);
    IO.Fonts->Build();
}

void render::NewFrame() {
    if (ImGui::IsKeyPressed(ImGuiKey_End, false)) utils::UnloadLibrary();

    esp::Render();
    menu::Render();
}

void render::Shutdown() {
    menu::Toggle(false);

    // Sleep to avoid crashes.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
