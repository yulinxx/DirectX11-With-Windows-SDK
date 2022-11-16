#include "Triangle.hlsli"

// SV_Target	说明输出的颜色值将会直接保存到渲染目标视图的后备缓冲区对应位置
// SV_前缀的变量代表system value的意思
// COLOR 在不同平台（索尼、ps5）可能会出现无法工作的情况，所以推荐使用SV_Target

// 像素着色器
float4 PS(VertexOut pIn) : SV_Target
{
    float dist = distance(pIn.flatPt, pIn.color);
    if(int(dist * 100) % 5 == 0)
        discard;

    return pIn.posH;
}

