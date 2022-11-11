#include "GameApp.h"


// DirectX11 With Windows SDK--26 计算着色器：入门 - X_Jun - 博客园
// https://www.cnblogs.com/X-Jun/p/10340652.html
// https://www.cnblogs.com/X-Jun/p/9028764.html

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
    _In_ LPSTR cmdLine, _In_ int showCmd)
{
    // 这些参数不使用
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(showCmd);
    // 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    GameApp theApp(hInstance, L"DirectX11 Initialization", 1280, 720);

    if (!theApp.Init())
        return 0;

    return theApp.Run();
}




