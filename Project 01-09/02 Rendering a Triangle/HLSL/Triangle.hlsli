
// POSITION	描述该变量是一个坐标点
// SV_POSITION	说明该顶点的位置在从顶点着色器输出后，后续的着色器都不能改变它的值，作为光栅化时最终确定的像素位置
// COLOR	描述该变量是一个颜色
// SV_Target	说明输出的颜色值将会直接保存到渲染目标视图的后备缓冲区对应位置
struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;  // SV = System Value float4类型，齐次坐标下的位置，即经过投影变换后的坐标
    float4 color : COLOR;
    nointerpolation float4 flatPt : TEXCOORD1; // 这里就是不插值的数据
};


