#include <stdio.h>
#include <Windows.h>

#include "console.hpp"

void console::Initialize() {
    AllocConsole();
    SetConsoleTitleA("cs2cheat debug console");
    freopen_s(reinterpret_cast<FILE**> stdin, "CONIN$", "r", stdin);
    freopen_s(reinterpret_cast<FILE**> stdout, "CONOUT$", "w", stdout);
}

void console::Shutdown() {
    fclose(stdin);
    fclose(stdout);
    FreeConsole();
}
