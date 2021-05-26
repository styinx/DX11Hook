cmake_minimum_required(VERSION 3.17)
project(DX11Hook)

set(CMAKE_CXX_STANDARD 14)

link_directories(
        ./
        ./cmake-build-debug/minhook
        ./cmake-build-debug/DLLInject)

include_directories(
        source/
        imgui/
        imgui/backends/
        minhook/include/
        DLLInject/source/
)

# Add Minhook
add_subdirectory(minhook)
set_target_properties(minhook PROPERTIES DEBUG_POSTFIX "")
set_target_properties(minhook PROPERTIES RELEASE_POSTFIX "")
set_target_properties(minhook PROPERTIES RELWITHDEBINFO_POSTFIX "")
set_target_properties(minhook PROPERTIES MINSIZEREL_POSTFIX "")
set_target_properties(minhook PROPERTIES PREFIX "lib")

# Add DLLInject
add_subdirectory(DLLInject)

# Add Demo
add_executable(demo demo/demo.cpp demo/DX11.cpp)
target_link_libraries(demo -ld3d11)

add_executable(inject demo/inject.cpp)
target_link_libraries(inject -lDLLInject)

# Add DLL
add_library(hook SHARED
        source/dll.cpp
        source/dx11hook.cpp
        imgui/backends/imgui_impl_win32.cpp
        imgui/backends/imgui_impl_dx11.cpp)
target_link_libraries(hook -ld3d11 -ld3dcompiler -lminhook -limgui -limm32 -ldwmapi)
