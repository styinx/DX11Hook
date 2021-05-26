#include "DX11.hpp"

DX11 dx11;

int main(int, char**)
{
    dx11 = DX11{};

    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        GetModuleHandle(nullptr),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        TEXT("Demo"),
        nullptr};

    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(
        wc.lpszClassName,
        TEXT("Demo"),
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        1280,
        800,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr);

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    dx11.createDevice(hwnd);

    bool done = false;
    while(!done)
    {
        MSG msg;
        while(::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if(msg.message == WM_QUIT)
                done = true;
        }
        if(done)
            break;

        // Clear
        dx11.getDeviceContext()->ClearRenderTargetView(
            dx11.getRenderTargetView(),
            D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

        // Draw

        // Present
        dx11.getSwapChain()->Present(0, 0);
    }

    dx11.cleanupDevice();

    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_SIZE:
        if(dx11.getDevice() != nullptr && wParam != SIZE_MINIMIZED)
        {
            dx11.cleanupRenderTarget();
            dx11.getSwapChain()->ResizeBuffers(
                0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            dx11.createRenderTarget();
        }
        return 0;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}