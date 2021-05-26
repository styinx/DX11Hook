#include "dx11hook.hpp"

DXHook hook{};

HRESULT __stdcall PresentHook(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags)
{
    if(!hook.ready)
    {
        std::cout << "Init ImGui" << std::endl;

        DXGI_SWAP_CHAIN_DESC description;
        swap_chain->GetDesc(&description);

        swap_chain->GetDevice(__uuidof(hook.device), (void**)&hook.device);
        hook.device->GetImmediateContext(&hook.device_context);
        hook.target_window = description.OutputWindow;

        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hook.target_window);
        ImGui_ImplDX11_Init(hook.device, hook.device_context);

        ID3D11Texture2D* back_buffer;
        swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
        hook.device->CreateRenderTargetView(back_buffer, nullptr, &hook.render_target);
        back_buffer->Release();

        hook.ready = true;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    bool bShow = true;
    ImGui::ShowDemoWindow(&bShow);

    ImGui::EndFrame();
    ImGui::Render();

    hook.device_context->OMSetRenderTargets(1, &hook.render_target, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return originalPresent(swap_chain, sync_interval, flags);
}

void initHook()
{
    auto windowed = ((GetWindowLongPtr(hook.target_window, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount                        = 1;
    sd.BufferDesc.Width                   = 1;
    sd.BufferDesc.Height                  = 1;
    sd.BufferDesc.RefreshRate.Numerator   = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.OutputWindow                       = hook.target_window;
    sd.SampleDesc.Count                   = 1;
    sd.SampleDesc.Quality                 = 0;
    sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
    sd.Windowed                           = windowed;

    D3D_FEATURE_LEVEL features_levels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1};
    D3D_FEATURE_LEVEL obtained_feature_level;
    if(FAILED(D3D11CreateDeviceAndSwapChain(
           nullptr,
           D3D_DRIVER_TYPE_HARDWARE,
           nullptr,
           0,
           features_levels,
           sizeof(features_levels) / sizeof(D3D_FEATURE_LEVEL),
           D3D11_SDK_VERSION,
           &sd,
           &hook.swap_chain,
           &hook.device,
           &obtained_feature_level,
           &hook.device_context)))
    {
        return;
    }

    hook.swap_chain_vtable = (DWORD_PTR*)(hook.swap_chain);
    hook.swap_chain_vtable = (DWORD_PTR*)(hook.swap_chain_vtable[0]);

    hook.device_vtable = (DWORD_PTR*)(hook.device);
    hook.device_vtable = (DWORD_PTR*)hook.device_vtable[0];

    hook.device_context_vtable = (DWORD_PTR*)(hook.device_context);
    hook.device_context_vtable = (DWORD_PTR*)(hook.device_context_vtable[0]);

    initMinHook();
}

void shutdownHook()
{
    hook.device->Release();
    hook.device_context->Release();
    hook.swap_chain->Release();

    shutdownMinHook();

    FreeLibraryAndExitThread(hook.target_module, 0);
}

UINT initMinHook()
{
    if(MH_Initialize() != MH_OK)
    {
        return 1;
    }

    if(MH_CreateHook(
           (DWORD_PTR*)hook.swap_chain_vtable[8],
           (DWORD_PTR*)PresentHook,
           reinterpret_cast<void**>(&originalPresent)) != MH_OK)
    {
        return 1;
    }
    if(MH_EnableHook((DWORD_PTR*)hook.swap_chain_vtable[8]) != MH_OK)
    {
        return 1;
    }
    return 0;
}

void shutdownMinHook()
{
    if(MH_DisableHook(MH_ALL_HOOKS))
        return;
    if(MH_Uninitialize())
        return;
}
