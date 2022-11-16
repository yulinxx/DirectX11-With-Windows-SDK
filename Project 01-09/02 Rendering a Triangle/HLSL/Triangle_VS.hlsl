#include "Triangle.hlsli"

// 顶点着色器
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    
    vOut.posH = float4(vIn.pos, 1.0f);
    vOut.color = vOut.posH; // 这里alpha通道的值默认为1.0
    vOut.flatPt = vOut.posH;

    return vOut;
}
