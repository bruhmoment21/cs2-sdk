#include <Windows.h>

#include "../utils/console/console.hpp"

// Defined in 'cs2cheat.cpp'.
void CS2_Initialize();
void CS2_Shutdown();

#ifdef IS_WINDOWS
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    // Console is initialized in DllMain because on linux you don't alloc a
    // console like you do on Windows.

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        console::Initialize();

        HANDLE hThread = CreateThread(
            NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(CS2_Initialize),
            NULL, 0, NULL);
        if (hThread != NULL) {
            CloseHandle(hThread);
        }
    } else if (fdwReason == DLL_PROCESS_DETACH && !lpvReserved) {
        CS2_Shutdown();

        console::Shutdown();
    }

    return TRUE;
}
#endif
