cbuffer cb
{
    matrix model;
    matrix view;
    matrix Invview;
    matrix proj;
};

struct VSIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct VSOut
{
    float3 Normal : NORMAL;
    float3 WorldPos : POSITION;
    float3 EyePos : EYEPOS;
    float4 ViewPos : SV_Position;
};

VSOut main(VSIn vsin)
{    
    VSOut vsout;
    vsout.WorldPos = (float3) mul(float4(vsin.pos, 1), model);
       
    vsout.ViewPos = mul(float4(vsin.pos, 1), model); // To world
    vsout.ViewPos = mul(vsout.ViewPos, view); // to view
    vsout.ViewPos = mul(vsout.ViewPos, proj); // NDC
    
    vsout.Normal = mul(vsin.normal, (float3x3)model);
    // Get eye position
    vsout.EyePos = float3(view[3].xyz);
    
	return vsout;  
}