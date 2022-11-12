#include "d3dUtil.h"

// ------------------------------
// CreateShaderFromFile函数
// ------------------------------
// [In]csoFileNameInOut 编译好的着色器二进制文件(.cso)，若有指定则优先寻找该文件并读取
// [In]hlslFileName     着色器代码，若未找到着色器二进制文件则编译着色器代码
// [In]entryPoint       入口点(指定开始的函数)
// [In]shaderModel      着色器模型，格式为"*s_5_0"，*可以为c,d,g,h,p,v之一
// [Out]ppBlobOut       输出着色器二进制信息

HRESULT CreateShaderFromFile(
    const WCHAR* csoFileNameInOut,
    const WCHAR* hlslFileName,
    LPCSTR entryPoint,
    LPCSTR shaderModel,
    ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    // 寻找是否有已经编译好的顶点着色器
    // D3DReadFileToBlob函数--读取编译好的着色器二进制信息 1 [In].cso文件名 2 [Out]获取二进制大数据块
    // https://www.cnblogs.com/X-Jun/p/10066282.html

    if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
    {
        return hr;
    }
    else
    {
        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        // 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
        // 但仍然允许着色器进行优化操作
        dwShaderFlags |= D3DCOMPILE_DEBUG;

        // 在Debug环境下禁用优化以避免出现一些不合理的情况
        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
        ID3DBlob* errorBlob = nullptr;
        hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
            dwShaderFlags, 0, ppBlobOut, &errorBlob);
        if (FAILED(hr))
        {
            if (errorBlob != nullptr)
            {
                OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
            }
            SAFE_RELEASE(errorBlob);
            return hr;
        }

        // 若指定了输出文件名，则将着色器二进制信息输出
        if (csoFileNameInOut)
        {
            return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
        }
    }

    return hr;
}
