#pragma once

#include <platform/platform.hpp>

class CConstants {
   public:
    static constexpr auto MATCHMAKING_LIB = platform::Constant("matchmaking.dll", "libmatchmaking.so");
    static constexpr auto CLIENT_LIB = platform::Constant("client.dll", "libclient.so");
    static constexpr auto ENGINE_LIB = platform::Constant("engine2.dll", "libengine2.so");
    static constexpr auto SDL_LIB = platform::Constant("SDL3.dll", "libSDL3.so.0");
    static constexpr auto INPUTSYSTEM_LIB = platform::Constant("inputsystem.dll", "libinputsystem.so");
    static constexpr auto SCHEMASYSTEM_LIB = platform::Constant("schemasystem.dll", "libschemasystem.so");
    static constexpr auto ANIMATIONSYSTEM_LIB = platform::Constant("animationsystem.dll", "libanimationsystem.so");
    static constexpr auto TIER_LIB = platform::Constant("tier0.dll", "libtier0.so");
    static constexpr auto RENDERSYSTEMVULKAN_LIB = platform::Constant("rendersystemvulkan.dll", "librendersystemvulkan.so");
};
