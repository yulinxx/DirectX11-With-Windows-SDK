#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;

#define DRAW_TRIANGLE  1

// 绘制过程
// 要在DX上绘制一个基本图形，大体流程有以下几步

// 给出输入布局（主要是描述顶点的格式）
// 给出描述几何体的顶点，索引，拓扑图元
// 创建并加载各种着色器
// 创建索引缓冲，顶点缓冲，以及必要的常量缓冲
// 将以上内容绑定到渲染管线，然后绘制
////////////////////////////////////////////////////////////////////////

// 建立C++结构体与HLSL结构体(VertexIn)的对应关系, 顶点数据描述结构体
// typedef struct D3D11_INPUT_ELEMENT_DESC
// {
//     LPCSTR SemanticName;        // 语义名 语义名要与HLSL结构体中的语义名相同, 用于将顶点结构体中的元素映射为顶点着色器参数
//     UINT SemanticIndex;         // 语义索引
//     DXGI_FORMAT Format;         // 数据格式
//     UINT InputSlot;             // 输入槽索引(0-15)
//     UINT AlignedByteOffset;     // 初始位置(字节偏移量)      
//     D3D11_INPUT_CLASSIFICATION InputSlotClass; // 输入类型
//     UINT InstanceDataStepRate;  // 忽略
// }     D3D11_INPUT_ELEMENT_DESC;

const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


////////////////////////////////////////////////////////////////////////

GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : D3DApp(hInstance, windowName, initWidth, initHeight)
{
}

GameApp::~GameApp()
{
}


bool GameApp::Init()
{
    // 初始化窗口 及 D3D
    if (!D3DApp::Init())
        return false;

    // 初始化着色器
    if (!InitEffect())
        return false;
    
    // 初始化数据
    if (!InitResource())
        return false;

    return true;
}

void GameApp::OnResize()
{
    D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{

}

void GameApp::DrawScene()
{
    assert(m_pd3dImmediateContext);
    assert(m_pSwapChain);

    static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
    m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
    m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // 绘制三角形
    // void ID3D11DeviceContext::Draw( 
    // UINT VertexCount,           // [In]需要绘制的顶点数目
    // UINT StartVertexLocation);  // [In]起始顶点索引

    //**********************************************
    // 从输入装配阶段开始，该绘制的进行将会经历一次完整的渲染管线阶段，直到输出合并阶段为止。
    m_pd3dImmediateContext->Draw(m_nDrawCount, 0);  // 渲染

    HR(m_pSwapChain->Present(0, 0));
}

bool GameApp::InitEffect()
{
    // I see ID3D10Blob as (I)(D3D10)(Blob).
    // I = interface? D3D10 = direct3d 10? Blob = ?
    // I've seen "Binary large object" online for Blob, 
    // but I'm not sure if that has the same meaning in this context.
    ComPtr<ID3DBlob> blob;

    // 使用CreateVertexShader/CreatePixelShader创建顶点和像素着色器。

    // 创建顶点着色器
    // HRESULT ID3D11Device::CreateVertexShader( 
    // const void *pShaderBytecode,            // [In]着色器字节码
    // SIZE_T BytecodeLength,                  // [In]字节码长度
    // ID3D11ClassLinkage *pClassLinkage,      // [In_Opt]忽略
    // ID3D11VertexShader **ppVertexShader);   // [Out]获取顶点着色器

    HR(CreateShaderFromFile(L"HLSL\\Triangle_VS.cso", L"HLSL\\Triangle_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
    
    // 创建并绑定顶点布局  创建顶点输入布局是为了要让着色器知道按照何种规则读取输入的顶点数据。 输入布局（主要是描述顶点的格式）
    // HRESULT ID3D11Device::CreateInputLayout( 
    // const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, // [In]输入布局描述
    // UINT NumElements,                                   // [In]上述数组元素个数
    // const void *pShaderBytecodeWithInputSignature,      // [In]顶点着色器字节码
    // SIZE_T BytecodeLength,                              // [In]顶点着色器字节码长度
    // ID3D11InputLayout **ppInputLayout);                 // [Out]获取的输入布局
    HR(m_pd3dDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));

    // 创建像素着色器
    HR(CreateShaderFromFile(L"HLSL\\Triangle_PS.cso", L"HLSL\\Triangle_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));


    return true;
}

bool GameApp::InitResource()
{
    // 设置三角形顶点 三个顶点的给出顺序应当按顺时针排布
    VertexPosColor vertices[] =
    {
        #if DRAW_TRIANGLE
        //           x     y     z              r     g     b    a
        { XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.8f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
        #else
        { XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.8f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(0.5f, 0.8f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(0.0f, 0.3f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
        #endif
    };

    m_nDrawCount = sizeof(vertices) / (sizeof(XMFLOAT3) + sizeof(XMFLOAT4));

    //**********************************************
    // 为了让GPU能够读取顶点, 他们需要被存放在一个名为buffer的结构里, 这个buffer由 ID3D11Buffer 定义. 
    // 存放顶点的buffer就叫做vertex buffer. 它在内存上的布局是一维线性的
    // 但是需要注意, D3D的buffer不仅仅存放数据, 数据的描述和绑定到渲染管线的位置也存储在这里.
    // 创建vertex buffer需要三个步骤:
    // 1. 设置 D3D11_BUFFER_DESC 结构来描述buffer.
    // 2. 设置 D3D11_SUBRESOURCE_DATA 结构, 他指定了我们想要初始化缓冲区内容的数据.
    // 3. 调用 ID3D11Device::CreateBuffer 创建 vertex buffer.

    // 设置顶点缓冲区描述
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;          // CPU和GPU的读写权限相关
    vbd.ByteWidth = sizeof vertices;            // 数据字节数
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;   // 缓冲区类型的标志
    vbd.CPUAccessFlags = 0;                     // CPU读写权限的指定

    // 新建顶点缓冲区 D3D11_SUBRESOURCE_DATA存放真实顶点数据
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;    // 用于初始化的数据 为缓冲区指定初始化数据

    // 创建顶点缓冲
    HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));    // 创建


    //**********************************************
    // 将 Vertex Buffer 绑定到 Input Slot, 给渲染管线各个阶段绑定好所需资源

    // 输入装配阶段的顶点缓冲区设置
    UINT stride = sizeof(VertexPosColor);	// 跨越字节数
    UINT offset = 0;						// 起始偏移量

    // 绑定顶点缓存到管线 用来设置一个或多个顶点缓存
    // 使用ID3D11DeviceContext::IASetVertexBuffers绑定到输入装配器阶段（Input-Assember Stage）
    // void ID3D11DeviceContext::IASetVertexBuffers( 
    // UINT StartSlot,     // [In]输入槽索引，就是传递过去的创建好的顶点缓存数组中的第一个要使用的缓存序号。序号从0到15.
    // UINT NumBuffers,    // [In]缓冲区数目,  要绑定的 buffer 的数量, 传进去的顶点缓存数量
    // ID3D11Buffer *const *ppVertexBuffers,   // [In]指向缓冲区数组的指针, 一个或多个缓存数组
    // const UINT *pStrides,   // [In]一个数组，规定了对所有缓冲区每次读取的字节数分别是多少，它表示每个顶点的字节数
    // const UINT *pOffsets);  // [In]一个数组，规定了对所有缓冲区的初始字节偏移量, 从缓存的开始位置到顶点的第一个元素的开始位置的字节偏移量
    m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset); // 设置

    // 设置图元拓扑 即设置图元类型，设定输入布局 
    // 图元拓扑表示告诉D3D，输入的顶点如何相连。即绘制什么图元
    #if DRAW_TRIANGLE
    m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // 绘制
    #else
    m_pd3dImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
    #endif
    // m_pd3dImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    
    // 绑定所创建的顶点布局，该函数只有一个单一的类型为 ID3D11InputLayout 的对象
    m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());

    // 绑定着色器对象到管线 将着色器绑定到渲染管线
    m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0); // 绑定
    m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

    // ******************
    // 设置调试对象名
    //
    D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
    D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
    D3D11SetDebugObjectName(m_pVertexShader.Get(), "Trangle_VS");
    D3D11SetDebugObjectName(m_pPixelShader.Get(), "Trangle_PS");

    return true;
}
