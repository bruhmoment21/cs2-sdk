#pragma once

#ifdef __linux__
#define LINUX_OFFSET(x) x
#else
#define LINUX_OFFSET(x) 0
#endif

class CConstants {
   public:
    static constexpr auto NAVSYSTEM_LIB = [] {
#ifdef _WIN32
        return "navsystem.dll";
#elif __linux__
        return "libscenefilecache.so";
#endif
    }();

    static constexpr auto CLIENT_LIB = [] {
#ifdef _WIN32
        return "client.dll";
#elif __linux__
        return "libclient.so";
#endif
    }();

    static constexpr auto ENGINE_LIB = [] {
#ifdef _WIN32
        return "engine2.dll";
#elif __linux__
        return "libengine2.so";
#endif
    }();

    static constexpr auto SDL_LIB = [] {
#ifdef _WIN32
        return "SDL3.dll";
#elif __linux__
        return "libSDL3.so.0";
#endif
    }();

    static constexpr auto INPUTSYSTEM_LIB = [] {
#ifdef _WIN32
        return "inputsystem.dll";
#elif __linux__
        return "libinputsystem.so";
#endif
    }();

    static constexpr auto SCHEMASYSTEM_LIB = [] {
#ifdef _WIN32
        return "schemasystem.dll";
#elif __linux__
        return "libschemasystem.so";
#endif
    }();

    static constexpr auto ANIMATIONSYSTEM_LIB = [] {
#ifdef _WIN32
        return "animationsystem.dll";
#elif __linux__
        return "libanimationsystem.so";
#endif
    }();

    static constexpr auto TIER_LIB = [] {
#ifdef _WIN32
        return "tier0.dll";
#elif __linux__
        return "libtier0.so";
#endif
    }();

    static constexpr auto RENDERSYSTEMVULKAN_LIB = [] {
#ifdef _WIN32
        return "rendersystemvulkan.dll";
#elif __linux__
        return "librendersystemvulkan.so";
#endif
    }();
};
