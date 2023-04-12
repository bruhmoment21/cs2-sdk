#include <stdio.h>
#include <Windows.h>

#include "console.hpp"

void console::Initialize() {
#ifdef CS2_SDK_ENABLE_LOGGING
    AllocConsole();
    SetConsoleTitleA("cs2cheat debug console");
    freopen_s(reinterpret_cast<FILE**> stdin, "CONIN$", "r", stdin);
    freopen_s(reinterpret_cast<FILE**> stdout, "CONOUT$", "w", stdout);
#endif
}

void console::Shutdown() {
#ifdef CS2_SDK_ENABLE_LOGGING
    fclose(stdin);
    fclose(stdout);
    FreeConsole();
#endif
}
