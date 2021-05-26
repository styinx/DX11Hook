#include "DLLInject.hpp"

int main(int argc, char** argv)
{
    DLLInject injector{"demo.exe", R"(C:\Users\Chris\Kruschd\DX11Hook\cmake-build-debug\libhook.dll)"};
    injector.run();
    return 0;
}