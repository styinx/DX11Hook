#ifndef D3D11_HOOK_H_INCLUDED_
#define D3D11_HOOK_H_INCLUDED_

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "MinHook.h"

#include <d3d11.h>
#include <iostream>
#include <windows.h>

/**
 * DX11 Stuff
 */

typedef HRESULT(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);

static Present originalPresent = nullptr;

/**
 * Hook Stuff
 */

void initHook();
void shutdownHook();

UINT initMinHook();
void shutdownMinHook();

struct DXHook
{
    bool    ready         = false;
    HMODULE target_module = nullptr;
    HWND    target_window = nullptr;

    ID3D11Device*           device         = nullptr;
    ID3D11DeviceContext*    device_context = nullptr;
    IDXGISwapChain*         swap_chain     = nullptr;
    ID3D11RenderTargetView* render_target  = nullptr;

    DWORD_PTR* swap_chain_vtable     = nullptr;
    DWORD_PTR* device_vtable         = nullptr;
    DWORD_PTR* device_context_vtable = nullptr;
};

extern DXHook      hook;
static const char* app_title = "Demo";

#endif  // D3D11_HOOK_H_INCLUDED_