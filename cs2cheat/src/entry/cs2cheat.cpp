#include <thread>

#include "../sdk/interfaces/interfaces.hpp"
#include "../sdk/memory/memory.hpp"
#include "../hooks/hooks.hpp"

void InitializeCheat(void* hInstance) {
    console::Initialize();
    interfaces::Initialize();
    memory::Initialize();
    hooks::Initialize();

    while (!GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    FreeLibraryAndExitThread((HMODULE)hInstance, 0);
}

void UninitializeCheat() {
    hooks::Shutdown();
    memory::Shutdown();
    interfaces::Shutdown();
    console::Shutdown();
}
