#include <thread>

#include "../sdk/interfaces/interfaces.hpp"
#include "../sdk/memory/memory.hpp"
#include "../hooks/hooks.hpp"

void CS2_Initialize() {
    CModule navsystem(NAVSYSTEM_DLL);
    while (!navsystem.IsLoaded()) {
        navsystem.Load();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    interfaces::Initialize();
    memory::Initialize();
    hooks::Initialize();
}

void CS2_Shutdown() {
    hooks::Shutdown();
    memory::Shutdown();
    interfaces::Shutdown();
}
