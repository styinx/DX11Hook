# DX11Hook
Hooks DX11 Present function to draw ImGui

## Build

```
mkdir build
cd build
cmake ..
```

## How to use

1. Build ImGui static library for win32 and dx11 backend 
2. Build MinHook static library
3. Build DLLInject static library
4. Build hook shared library
5. Build inject executable
6. Build demo executable
7. Execute inject
8. Execute demo

## Result

![Demo No Hook](https://github.com/styinx/DLLInject/blob/main/demo_nohook.png | width=300)
![Demo Hook](https://github.com/styinx/DLLInject/blob/main/demo_hook.png | width=300)
![Game Hook](https://github.com/styinx/DLLInject/blob/main/game_hook.png | width=300)
