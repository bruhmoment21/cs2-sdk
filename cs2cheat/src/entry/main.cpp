#include <Windows.h>

// Defined in 'cs2cheat.cpp'.
void InitializeCheat(void* hInstance);
void UninitializeCheat();

#ifdef IS_WINDOWS
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        HANDLE hThread = CreateThread(
            NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(InitializeCheat),
            hinstDLL, 0, NULL);
        if (hThread != NULL) {
            CloseHandle(hThread);
        }
    } else if (fdwReason == DLL_PROCESS_DETACH && !lpvReserved) {
        UninitializeCheat();
    }

    return TRUE;
}
#endif
