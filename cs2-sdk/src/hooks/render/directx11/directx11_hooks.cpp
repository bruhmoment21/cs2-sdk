#include "pch.hpp"

#include <hooks/render/directx11/directx11_hooks.hpp>
#include <logger/logger.hpp>

#include <hooks/input/wndproc/wndproc_hooks.hpp>

#include <renderer/renderer.hpp>

#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <hook/hook.hpp>

#include <d3d11.h>
#include <dxgi1_2.h>

static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;
static ID3D11RenderTargetView* g_RenderTarget = NULL;
static IDXGISwapChain* g_SwapChain = NULL;

static auto GetCorrectDXGIFormat(DXGI_FORMAT currentFormat) {
    switch (currentFormat) {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
    }

    return currentFormat;
}

static bool CreateDevice(HWND hWnd) {
    SDK_LOG_PROLOGUE();

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;

    constexpr D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_NULL, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc,
                                               &g_SwapChain, &g_Device, nullptr, nullptr);

    return hr == S_OK;
}

static void CreateRenderTarget(IDXGISwapChain* pSwapChain) {
    SDK_LOG_PROLOGUE();

    ID3D11Texture2D* pBackBuffer = NULL;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer) {
        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);

        D3D11_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format = GetCorrectDXGIFormat(sd.BufferDesc.Format);

        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
        if (FAILED(g_Device->CreateRenderTargetView(pBackBuffer, &desc, &g_RenderTarget))) {
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            if (FAILED(g_Device->CreateRenderTargetView(pBackBuffer, &desc, &g_RenderTarget))) {
                if (FAILED(g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_RenderTarget))) {
                    CLogger::Log("[dx11] CreateRenderTargetView() failed!");
                }
            }
        }

        pBackBuffer->Release();
    }
}

static void CleanupRenderTarget() {
    SDK_LOG_PROLOGUE();

    if (g_RenderTarget) {
        g_RenderTarget->Release();
        g_RenderTarget = NULL;
    }
}

static void CleanupDevice() {
    SDK_LOG_PROLOGUE();

    CleanupRenderTarget();

    if (g_SwapChain) {
        g_SwapChain->Release();
        g_SwapChain = NULL;
    }
    if (g_Device) {
        g_Device->Release();
        g_Device = NULL;
    }
    if (g_DeviceContext) {
        g_DeviceContext->Release();
        g_DeviceContext = NULL;
    }
}

static void RenderImGui(IDXGISwapChain* pSwapChain) {
    if (!ImGui::GetCurrentContext()) return;

    if (!ImGui::GetIO().BackendRendererUserData) {
        if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_Device)))) {
            g_Device->GetImmediateContext(&g_DeviceContext);
            ImGui_ImplDX11_Init(g_Device, g_DeviceContext);

            CRenderer::Get().Initialize();
        } else {
            return CLogger::Log("[dx11] GetDevice() failed!");
        }
    }

    if (!g_RenderTarget) {
        CreateRenderTarget(pSwapChain);
    } else {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        CRenderer::Get().NewFrame();

        ImGui::Render();
        g_DeviceContext->OMSetRenderTargets(1, &g_RenderTarget, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

static CHook g_Present;
static HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    RenderImGui(pSwapChain);

    return g_Present.CallOriginal<HRESULT>(pSwapChain, SyncInterval, Flags);
}

static CHook g_Present1;
static HRESULT WINAPI hkPresent1(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT PresentFlags,
                                 const DXGI_PRESENT_PARAMETERS* pPresentParameters) {
    RenderImGui(pSwapChain);

    return g_Present1.CallOriginal<HRESULT>(pSwapChain, SyncInterval, PresentFlags, pPresentParameters);
}

static CHook g_ResizeBuffers;
static HRESULT WINAPI hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat,
                                      UINT SwapChainFlags) {
    CleanupRenderTarget();

    return g_ResizeBuffers.CallOriginal<HRESULT>(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

static CHook g_ResizeBuffers1;
static HRESULT WINAPI hkResizeBuffers1(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat,
                                       UINT SwapChainFlags, const UINT* pCreationNodeMask, IUnknown* const* ppPresentQueue) {
    CleanupRenderTarget();

    return g_ResizeBuffers1.CallOriginal<HRESULT>(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags, pCreationNodeMask,
                                                  ppPresentQueue);
}

static CHook g_CreateSwapChain;
static HRESULT WINAPI hkCreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc,
                                        IDXGISwapChain** ppSwapChain) {
    CleanupRenderTarget();

    return g_CreateSwapChain.CallOriginal<HRESULT>(pFactory, pDevice, pDesc, ppSwapChain);
}

static CHook g_CreateSwapChainForHwnd;
static HRESULT WINAPI hkCreateSwapChainForHwnd(IDXGIFactory* pFactory, IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc,
                                               const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput,
                                               IDXGISwapChain1** ppSwapChain) {
    CleanupRenderTarget();

    return g_CreateSwapChainForHwnd.CallOriginal<HRESULT>(pFactory, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
}

static CHook g_CreateSwapChainForCoreWindow;
static HRESULT WINAPI hkCreateSwapChainForCoreWindow(IDXGIFactory* pFactory, IUnknown* pDevice, IUnknown* pWindow,
                                                     const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput,
                                                     IDXGISwapChain1** ppSwapChain) {
    CleanupRenderTarget();

    return g_CreateSwapChainForCoreWindow.CallOriginal<HRESULT>(pFactory, pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
}

static CHook g_CreateSwapChainForComposition;
static HRESULT WINAPI hkCreateSwapChainForComposition(IDXGIFactory* pFactory, IUnknown* pDevice, const DXGI_SWAP_CHAIN_DESC1* pDesc,
                                                      IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) {
    CleanupRenderTarget();

    return g_CreateSwapChainForComposition.CallOriginal<HRESULT>(pFactory, pDevice, pDesc, pRestrictToOutput, ppSwapChain);
}

void CDirectX11Hooks::Initialize() {
    SDK_LOG_PROLOGUE();

    if (!CreateDevice(CWndProcHooks::Get().GetWindow())) {
        return CLogger::Log("[dx11] CreateDevice() failed!");
    }

    IDXGIDevice* pDXGIDevice = NULL;
    g_Device->QueryInterface(IID_PPV_ARGS(&pDXGIDevice));

    IDXGIAdapter* pDXGIAdapter = NULL;
    pDXGIDevice->GetAdapter(&pDXGIAdapter);

    IDXGIFactory* pIDXGIFactory = NULL;
    pDXGIAdapter->GetParent(IID_PPV_ARGS(&pIDXGIFactory));

    g_CreateSwapChain.VHook(pIDXGIFactory, 10, SDK_HOOK(hkCreateSwapChain));
    g_CreateSwapChainForHwnd.VHook(pIDXGIFactory, 15, SDK_HOOK(hkCreateSwapChainForHwnd));
    g_CreateSwapChainForCoreWindow.VHook(pIDXGIFactory, 16, SDK_HOOK(hkCreateSwapChainForCoreWindow));
    g_CreateSwapChainForComposition.VHook(pIDXGIFactory, 24, SDK_HOOK(hkCreateSwapChainForComposition));

    g_Present.VHook(g_SwapChain, 8, SDK_HOOK(hkPresent));
    g_Present1.VHook(g_SwapChain, 22, SDK_HOOK(hkPresent1));

    g_ResizeBuffers.VHook(g_SwapChain, 13, SDK_HOOK(hkResizeBuffers));
    g_ResizeBuffers1.VHook(g_SwapChain, 39, SDK_HOOK(hkResizeBuffers1));

    pIDXGIFactory->Release();
    pDXGIAdapter->Release();
    pDXGIDevice->Release();

    CleanupDevice();
}

CDirectX11Hooks::~CDirectX11Hooks() {
    SDK_LOG_PROLOGUE();

    if (ImGui::GetCurrentContext()) {
        ImGuiIO& io = ImGui::GetIO();

        if (io.BackendRendererUserData) {
            ImGui_ImplDX11_Shutdown();
        }

        if (io.BackendPlatformUserData) {
            ImGui_ImplWin32_Shutdown();
        }

        ImGui::DestroyContext();
    }

    CleanupDevice();
}
