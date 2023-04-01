#include <thread>

#include "../sdk/interfaces/interfaces.hpp"
#include "../sdk/memory/memory.hpp"
#include "../hooks/hooks.hpp"

void InitializeCheat() {
    CModule navsystem(NAVSYSTEM_DLL, true);
    while (!navsystem.IsLoaded()) {
        navsystem.Load();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    console::Initialize();
    interfaces::Initialize();
    memory::Initialize();
    hooks::Initialize();
}

void UninitializeCheat() {
    hooks::Shutdown();
    memory::Shutdown();
    interfaces::Shutdown();
    console::Shutdown();
}
