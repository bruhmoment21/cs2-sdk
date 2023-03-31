#include <Windows.h>
#include <thread>

#include "../../../game/menu/menu.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>

static BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);

static HWND g_hWindow;
static WNDPROC g_oWndProc;
static LRESULT hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (!ImGui::GetCurrentContext()) {
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hWnd);
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = io.LogFilename = nullptr;
    }

    LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
                                           LPARAM lParam);
    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
    if (menu::IsOpen()) {
        // Messages handled by 'ImGui_ImplWin32_WndProcHandler'.
        switch (uMsg) {
            case WM_MOUSEMOVE:
            case WM_NCMOUSEMOVE:
            case WM_MOUSELEAVE:
            case WM_NCMOUSELEAVE:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONDBLCLK:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONDBLCLK:
            case WM_XBUTTONDOWN:
            case WM_XBUTTONDBLCLK:
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            case WM_XBUTTONUP:
            case WM_MOUSEWHEEL:
            case WM_MOUSEHWHEEL:
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_SETFOCUS:
            case WM_KILLFOCUS:
            case WM_CHAR:
            case WM_SETCURSOR:
            case WM_DEVICECHANGE:
                return 1;
        }
    }

    return CallWindowProc(g_oWndProc, hWnd, uMsg, wParam, lParam);
}

void CS2_HookInputAPI() {
    while (!g_hWindow) {
        EnumWindows(::EnumWindowsCallback,
                    reinterpret_cast<LPARAM>(&g_hWindow));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    g_oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
        g_hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));
}

void CS2_UnhookInputAPI() {
    if (g_oWndProc) {
        SetWindowLongPtr(g_hWindow, GWLP_WNDPROC,
                         reinterpret_cast<LONG_PTR>(g_oWndProc));
    }
}

static BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam) {
    const auto isMainWindow = [handle]() {
        return GetWindow(handle, GW_OWNER) == nullptr &&
               IsWindowVisible(handle) && handle != GetConsoleWindow();
    };

    DWORD pID = 0;
    GetWindowThreadProcessId(handle, &pID);

    if (GetCurrentProcessId() != pID || !isMainWindow()) return TRUE;

    *reinterpret_cast<HWND *>(lParam) = handle;
    return FALSE;
}
