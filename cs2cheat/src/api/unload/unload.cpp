#include <Windows.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

void CS2_UnloadLibrary() {
    HANDLE hThread = CreateThread(
        NULL, 0,
        [](LPVOID pData) -> DWORD {
            FreeLibraryAndExitThread((HMODULE)(pData), EXIT_SUCCESS);
        },
        HINST_THISCOMPONENT, 0, NULL);

    if (hThread != NULL) {
        CloseHandle(hThread);
    }
}
