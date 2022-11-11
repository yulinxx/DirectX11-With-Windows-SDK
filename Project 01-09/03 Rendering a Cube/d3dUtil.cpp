#include "d3dUtil.h"


HRESULT CreateShaderFromFile(
    const WCHAR* csoFileNameInOut,
    const WCHAR* hlslFileName,
    LPCSTR entryPoint,
    LPCSTR shaderModel,
    ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    // 寻找是否有已经编译好的顶点着色器
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

        // DirectX11--HLSL编译着色器的三种方法 - X_Jun - 博客园
        // https://www.cnblogs.com/X-Jun/p/10066282.html#desc=true&!comments

        // HRESULT D3DCompileFromFile(
        // LPCWSTR pFileName,                  // [In]要编译的.hlsl文件
        // CONST D3D_SHADER_MACRO* pDefines,   // [In_Opt]忽略
        // ID3DInclude* pInclude,              // [In_Opt]如何应对#include宏
        // LPCSTR pEntrypoint,                 // [In]入口函数名
        // LPCSTR pTarget,                     // [In]使用的着色器模型
        // UINT Flags1,                        // [In]D3DCOMPILE系列宏
        // UINT Flags2,                        // [In]D3DCOMPILE_FLAGS2系列宏
        // ID3DBlob** ppCode,                  // [Out]获得着色器的二进制块
        // ID3DBlob** ppErrorMsgs);            // [Out]可能会获得错误信息的二进制块

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
            // D3DWriteBlobToFile函数--将编译好的着色器二进制信息写入文件
            return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
        }
    }

    return hr;
}

