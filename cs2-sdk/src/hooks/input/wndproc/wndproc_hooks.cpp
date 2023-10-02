#include "pch.hpp"

#include <hooks/input/wndproc/wndproc_hooks.hpp>
#include <logger/logger.hpp>

#include <menu/menu.hpp>

#include <imgui/imgui_impl_win32.h>

static WNDPROC g_WndProc;
static std::once_flag g_InputInit;

static LRESULT hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    std::call_once(g_InputInit, [hWnd]() {
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hWnd);
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = io.LogFilename = nullptr;
    });

    LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
    if (CMenu::Get().IsOpen()) {
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

    return CallWindowProc(g_WndProc, hWnd, uMsg, wParam, lParam);
}

static BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam) {
    const auto isMainWindow = [handle]() {
        return GetWindow(handle, GW_OWNER) == nullptr && IsWindowVisible(handle) && handle != GetConsoleWindow();
    };

    DWORD pID = 0;
    GetWindowThreadProcessId(handle, &pID);

    if (GetCurrentProcessId() != pID || !isMainWindow()) {
        return TRUE;
    }

    *reinterpret_cast<HWND *>(lParam) = handle;
    return FALSE;
}

void CWndProcHooks::Initialize() {
    SDK_LOG_PROLOGUE();

    EnumWindows(::EnumWindowsCallback, reinterpret_cast<LPARAM>(&m_Window));
    if (!m_Window) {
        return CLogger::Log("[wndproc] EnumWindows() returned nullptr!");
    }

    CLogger::Log("[wndproc] CWndProcHooks::m_Window -> {}", SDK_LOG_PTR(m_Window));
    g_WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));
}

void CWndProcHooks::Shutdown() {
    SDK_LOG_PROLOGUE();

    if (g_WndProc) {
        SetWindowLongPtr(m_Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_WndProc));
    }
}
