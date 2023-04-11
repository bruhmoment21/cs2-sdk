#include <thread>

#include "../../utils/utils.hpp"

#include "../menu/menu.hpp"
#include "../esp/esp.hpp"

#include "render.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_freetype.h>

#include <cozette/cozette_bitmap.hpp>

void render::Initialize() {
    ImGuiIO& IO = ImGui::GetIO();

    ImVector<ImWchar> ranges;
    ImFontGlyphRangesBuilder builder;

    // Add your custom ranges right here.
    // https://github.com/ocornut/imgui/blob/master/docs/FONTS.md#using-custom-glyph-ranges

    builder.AddRanges(IO.Fonts->GetGlyphRangesCyrillic());
    builder.BuildRanges(&ranges);

    ImFontConfig cfg{};
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Bitmap;
    cfg.SizePixels = 13.f;

    IO.Fonts->Clear();
    IO.Fonts->AddFontFromMemoryCompressedBase85TTF(
        cozette_bitmap_compressed_data_base85, 0.f, &cfg, ranges.Data);
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
