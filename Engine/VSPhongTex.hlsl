cbuffer cb
{
    matrix model;
    matrix InvView;
    matrix modelViewProj;
};

struct VSIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOut
{
    float3 Normal : NORMAL;
    float3 WorldPos : POSITION;
    float3 EyePos : EYEPOS;
    float2 texcoord : TEXCOORD;
    float4 ViewPos : SV_Position;
};

VSOut main(VSIn vsin)
{
    VSOut vsout;
    vsout.WorldPos = (float3) mul(float4(vsin.pos, 1), model);
       
    vsout.ViewPos = mul(float4(vsin.pos, 1), modelViewProj);
    vsout.texcoord = (float2)mul(float4(vsin.texcoord, 0, 1), model);
    vsout.Normal = mul(vsin.normal, (float3x3) model);
    // Get eye position
    vsout.EyePos = float3(InvView[3].xyz);
    
    return vsout;
}