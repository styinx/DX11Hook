#include "DX11.hpp"

bool DX11::createDevice(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount                        = 1;
    sd.BufferDesc.Width                   = 0;
    sd.BufferDesc.Height                  = 0;
    sd.BufferDesc.RefreshRate.Numerator   = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.OutputWindow                       = hWnd;
    sd.SampleDesc.Count                   = 1;
    sd.SampleDesc.Quality                 = 0;
    sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
    sd.Windowed                           = TRUE;

    D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1};
    D3D_FEATURE_LEVEL obtainedLevel;
    if(D3D11CreateDeviceAndSwapChain(
           nullptr,
           D3D_DRIVER_TYPE_HARDWARE,
           nullptr,
           0,
           featureLevels,
           2,
           D3D11_SDK_VERSION,
           &sd,
           &m_swap_chain,
           &m_device,
           &obtainedLevel,
           &m_device_context) != S_OK)
        return false;

    createRenderTarget();
    return true;
}

void DX11::cleanupDevice()
{
    cleanupRenderTarget();
    if(m_swap_chain)
    {
        m_swap_chain->Release();
        m_swap_chain = nullptr;
    }
    if(m_device_context)
    {
        m_device_context->Release();
        m_device_context = nullptr;
    }
    if(m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }
}

void DX11::createRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    m_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    m_device->CreateRenderTargetView(pBackBuffer, nullptr, &m_render_target);
    pBackBuffer->Release();
}

void DX11::cleanupRenderTarget()
{
    if(m_render_target)
    {
        m_render_target->Release();
        m_render_target = nullptr;
    }
}

ID3D11Device* DX11::getDevice()
{
    return m_device;
}

ID3D11DeviceContext* DX11::getDeviceContext()
{
    return m_device_context;
}

ID3D11RenderTargetView* DX11::getRenderTargetView()
{
    return m_render_target;
}

IDXGISwapChain* DX11::getSwapChain()
{
    return m_swap_chain;
}
