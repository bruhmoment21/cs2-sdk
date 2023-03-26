#include <Windows.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>

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
    return CallWindowProc(g_oWndProc, hWnd, uMsg, wParam, lParam);
}

void CS2_HookInputAPI() {
    // CS2TODO: I should probably find another solution.
    g_hWindow = FindWindow(NULL, "Counter-Strike 2");
    g_oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
        g_hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));
}

void CS2_UnhookInputAPI() {
    if (g_oWndProc) {
        SetWindowLongPtr(g_hWindow, GWLP_WNDPROC,
                         reinterpret_cast<LONG_PTR>(g_oWndProc));
    }
}
