#ifndef DX11_HPP
#define DX11_HPP

#include <d3d11.h>
#include <d3dx9math.h>
#include <windows.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class DX11
{
private:
    ID3D11Device*           m_device         = nullptr;
    ID3D11DeviceContext*    m_device_context = nullptr;
    ID3D11RenderTargetView* m_render_target  = nullptr;
    IDXGISwapChain*         m_swap_chain     = nullptr;

public:
    bool createDevice(HWND hWnd);
    void cleanupDevice();
    void createRenderTarget();
    void cleanupRenderTarget();

    ID3D11Device*           getDevice();
    ID3D11DeviceContext*    getDeviceContext();
    ID3D11RenderTargetView* getRenderTargetView();
    IDXGISwapChain*         getSwapChain();
};

#endif  // DX11_HPP
