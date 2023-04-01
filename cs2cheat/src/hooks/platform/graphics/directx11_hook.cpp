#include <Windows.h>

#include <d3d11.h>
#include <dxgi1_2.h>

#include "../../../game/render/render.hpp"
#include "../../../console/console.hpp"
#include "../../../api/hook/hook.hpp"

#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static ID3D11RenderTargetView* g_pd3dRenderTarget = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;

static void CleanupDeviceD3D11();
static void CleanupRenderTarget();
static void RenderImGui_DX11(IDXGISwapChain* pSwapChain);

static bool CreateDeviceD3D11(HWND hWnd) {
    // Create the D3DDevice
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;

    const D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_NULL, NULL, 0, featureLevels, 2,
        D3D11_SDK_VERSION, &swapChainDesc, &g_pSwapChain, &g_pd3dDevice,
        nullptr, nullptr);
    if (hr != S_OK) {
        LOG("[!] D3D11CreateDeviceAndSwapChain() failed. [rv: %lu]\n", hr);
        return false;
    }

    return true;
}

static int GetCorrectDXGIFormat(int eCurrentFormat) {
    switch (eCurrentFormat) {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
    }

    return eCurrentFormat;
}

static void CreateRenderTarget(IDXGISwapChain* pSwapChain) {
    ID3D11Texture2D* pBackBuffer = NULL;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer) {
        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);

        D3D11_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format = static_cast<DXGI_FORMAT>(
            GetCorrectDXGIFormat(sd.BufferDesc.Format));

        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
        if (FAILED(g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &desc,
                                                        &g_pd3dRenderTarget))) {
            LOG("CreateRenderTarget(): Falling back to "
                "'D3D11_RTV_DIMENSION_TEXTURE2D'.\n");

            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            if (FAILED(g_pd3dDevice->CreateRenderTargetView(
                    pBackBuffer, &desc, &g_pd3dRenderTarget))) {
                if (FAILED(g_pd3dDevice->CreateRenderTargetView(
                        pBackBuffer, NULL, &g_pd3dRenderTarget))) {
                    LOG("Severe error in CreateRenderTarget().\n");
                }
            }
        }

        pBackBuffer->Release();
    }
}

static CHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT)> g_present;
static HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval,
                                UINT Flags) {
    RenderImGui_DX11(pSwapChain);

    return g_present.m_pOriginalFn(pSwapChain, SyncInterval, Flags);
}

static CHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT,
                            const DXGI_PRESENT_PARAMETERS*)>
    g_present1;
static HRESULT WINAPI
hkPresent1(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT PresentFlags,
           const DXGI_PRESENT_PARAMETERS* pPresentParameters) {
    RenderImGui_DX11(pSwapChain);

    return g_present1.m_pOriginalFn(pSwapChain, SyncInterval, PresentFlags,
                                    pPresentParameters);
}

static CHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT,
                            UINT)>
    g_resizeBuffers;
static HRESULT WINAPI hkResizeBuffers(IDXGISwapChain* pSwapChain,
                                      UINT BufferCount, UINT Width, UINT Height,
                                      DXGI_FORMAT NewFormat,
                                      UINT SwapChainFlags) {
    CleanupRenderTarget();

    return g_resizeBuffers.m_pOriginalFn(pSwapChain, BufferCount, Width, Height,
                                         NewFormat, SwapChainFlags);
}

static CHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT,
                            UINT, const UINT*, IUnknown* const*)>
    g_resizeBuffers1;
static HRESULT WINAPI hkResizeBuffers1(IDXGISwapChain* pSwapChain,
                                       UINT BufferCount, UINT Width,
                                       UINT Height, DXGI_FORMAT NewFormat,
                                       UINT SwapChainFlags,
                                       const UINT* pCreationNodeMask,
                                       IUnknown* const* ppPresentQueue) {
    CleanupRenderTarget();

    return g_resizeBuffers1.m_pOriginalFn(pSwapChain, BufferCount, Width,
                                          Height, NewFormat, SwapChainFlags,
                                          pCreationNodeMask, ppPresentQueue);
}

static CHook<HRESULT WINAPI(IDXGIFactory*, IUnknown*, DXGI_SWAP_CHAIN_DESC*,
                            IDXGISwapChain**)>
    g_createSwapChain;
static HRESULT WINAPI hkCreateSwapChain(IDXGIFactory* pFactory,
                                        IUnknown* pDevice,
                                        DXGI_SWAP_CHAIN_DESC* pDesc,
                                        IDXGISwapChain** ppSwapChain) {
    CleanupRenderTarget();

    return g_createSwapChain.m_pOriginalFn(pFactory, pDevice, pDesc,
                                           ppSwapChain);
}

static CHook<HRESULT WINAPI(
    IDXGIFactory*, IUnknown*, HWND, const DXGI_SWAP_CHAIN_DESC1*,
    const DXGI_SWAP_CHAIN_FULLSCREEN_DESC*, IDXGIOutput*, IDXGISwapChain1**)>
    g_createSwapChainForHwnd;
static HRESULT WINAPI hkCreateSwapChainForHwnd(
    IDXGIFactory* pFactory, IUnknown* pDevice, HWND hWnd,
    const DXGI_SWAP_CHAIN_DESC1* pDesc,
    const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc,
    IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) {
    CleanupRenderTarget();

    return g_createSwapChainForHwnd.m_pOriginalFn(
        pFactory, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput,
        ppSwapChain);
}

static CHook<HRESULT WINAPI(IDXGIFactory*, IUnknown*, IUnknown*,
                            const DXGI_SWAP_CHAIN_DESC1*, IDXGIOutput*,
                            IDXGISwapChain1**)>
    g_createSwapChainForCoreWindow;
static HRESULT WINAPI hkCreateSwapChainForCoreWindow(
    IDXGIFactory* pFactory, IUnknown* pDevice, IUnknown* pWindow,
    const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput,
    IDXGISwapChain1** ppSwapChain) {
    CleanupRenderTarget();

    return g_createSwapChainForCoreWindow.m_pOriginalFn(
        pFactory, pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
}

static CHook<HRESULT WINAPI(IDXGIFactory*, IUnknown*,
                            const DXGI_SWAP_CHAIN_DESC1*, IDXGIOutput*,
                            IDXGISwapChain1**)>
    g_createSwapChainForComposition;
static HRESULT WINAPI hkCreateSwapChainForComposition(
    IDXGIFactory* pFactory, IUnknown* pDevice,
    const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput,
    IDXGISwapChain1** ppSwapChain) {
    CleanupRenderTarget();

    return g_createSwapChainForComposition.m_pOriginalFn(
        pFactory, pDevice, pDesc, pRestrictToOutput, ppSwapChain);
}

void CS2_HookDX11GraphicsAPI() {
    if (!CreateDeviceD3D11(GetConsoleWindow())) {
        LOG("[!] CreateDeviceD3D11() failed.\n");
        return;
    }

    // Hook
    IDXGIDevice* pDXGIDevice = NULL;
    g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pDXGIDevice));

    IDXGIAdapter* pDXGIAdapter = NULL;
    pDXGIDevice->GetAdapter(&pDXGIAdapter);

    IDXGIFactory* pIDXGIFactory = NULL;
    pDXGIAdapter->GetParent(IID_PPV_ARGS(&pIDXGIFactory));

    pIDXGIFactory->Release();
    pDXGIAdapter->Release();
    pDXGIDevice->Release();

    g_createSwapChain.HookVirtual(pIDXGIFactory, 10,
                                  HOOK_FUNCTION(hkCreateSwapChain));
    g_createSwapChainForHwnd.HookVirtual(
        pIDXGIFactory, 15, HOOK_FUNCTION(hkCreateSwapChainForHwnd));
    g_createSwapChainForCoreWindow.HookVirtual(
        pIDXGIFactory, 16, HOOK_FUNCTION(hkCreateSwapChainForCoreWindow));
    g_createSwapChainForComposition.HookVirtual(
        pIDXGIFactory, 24, HOOK_FUNCTION(hkCreateSwapChainForComposition));

    g_present.HookVirtual(g_pSwapChain, 8, HOOK_FUNCTION(hkPresent));
    g_present1.HookVirtual(g_pSwapChain, 22, HOOK_FUNCTION(hkPresent1));

    g_resizeBuffers.HookVirtual(g_pSwapChain, 13,
                                HOOK_FUNCTION(hkResizeBuffers));
    g_resizeBuffers1.HookVirtual(g_pSwapChain, 39,
                                 HOOK_FUNCTION(hkResizeBuffers1));

    CleanupDeviceD3D11();
}

void CS2_UnhookDX11GraphicsAPI() {
    if (!ImGui::GetCurrentContext()) return;

    render::Shutdown();

    ImGuiIO& io = ImGui::GetIO();
    if (io.BackendRendererUserData) ImGui_ImplDX11_Shutdown();

    if (io.BackendPlatformUserData) ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}

static void CleanupRenderTarget() {
    if (g_pd3dRenderTarget) {
        g_pd3dRenderTarget->Release();
        g_pd3dRenderTarget = NULL;
    }
}

static void CleanupDeviceD3D11() {
    CleanupRenderTarget();

    if (g_pSwapChain) {
        g_pSwapChain->Release();
        g_pSwapChain = NULL;
    }
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = NULL;
    }
    if (g_pd3dDeviceContext) {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = NULL;
    }
}

static void RenderImGui_DX11(IDXGISwapChain* pSwapChain) {
    if (!ImGui::GetCurrentContext() || ::g_isShuttingDown) return;

    if (!ImGui::GetIO().BackendRendererUserData) {
        if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
            g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
            ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

            render::Initialize();
        }
    }

    if (!g_pd3dRenderTarget) {
        CreateRenderTarget(pSwapChain);
    } else {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        render::NewFrame();

        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pd3dRenderTarget, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}
