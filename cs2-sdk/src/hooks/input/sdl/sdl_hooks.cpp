#include "pch.hpp"

#include <hooks/input/sdl/sdl_hooks.hpp>
#include <logger/logger.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

#include <menu/menu.hpp>
#include <hook/hook.hpp>

#include <imgui/imgui_impl_sdl3.h>

#include <SDL3/SDL.h>

static std::once_flag g_InputInit;

static CHook g_PeepEvents;
static int hkPeepEvents(SDL_Event* events, int numevents, SDL_eventaction action, Uint32 minType, Uint32 maxType) {
    int rv = g_PeepEvents.CallOriginal<int>(events, numevents, action, minType, maxType);
    if (rv == 0) return rv;

    for (int i = 0; i < numevents; ++i) {
        ImGuiContext* ctx = ImGui::GetCurrentContext();
        SDL_Event& event = events[i];
        if (!ctx && SDL_EVENT_WINDOW_FIRST <= event.type && event.type <= SDL_EVENT_WINDOW_LAST) {
            std::call_once(g_InputInit, [event]() {
                ImGui::CreateContext();

                SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
                CLogger::Log("[sdl] SDL_Window -> {}", SDK_LOG_PTR(window));

                ImGui_ImplSDL3_InitForVulkan(window);

                ImGuiIO& io = ImGui::GetIO();
                io.IniFilename = io.LogFilename = nullptr;
            });
        } else if (ctx) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (CMenu::Get().IsOpen()) {
                event.type = SDL_EVENT_FIRST;
            }
        }
    }

    return rv;
}

void CSDLHooks::Initialize() {
    SDK_LOG_PROLOGUE();

    auto peepEvents = CMemory::GetProcAddress(CConstants::SDL_LIB, "SDL_PeepEvents");
    g_PeepEvents.Hook(peepEvents.Get<void*>(), SDK_HOOK(hkPeepEvents));
}
