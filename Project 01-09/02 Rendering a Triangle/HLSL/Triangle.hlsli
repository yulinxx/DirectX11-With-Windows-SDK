
//  在dx11中，顶点由空间位置和各种附加属性组成，D3D允许我们灵活地建立属于自己的顶点格式。
// 要创建一个自定义的顶点格式，必须创建一个包含顶点数据的结构体。
// 如下VertexIn,顶点格式由位置和颜色组成
// POSITION	描述该变量是一个坐标点
// COLOR	描述该变量是一个颜色
// SV_POSITION	说明该顶点的位置在从顶点着色器输出后，后续的着色器都不能改变它的值，作为光栅化时最终确定的像素位置
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


