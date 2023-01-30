cbuffer cb
{
    matrix model;
    matrix modelviewproj;
};

struct Interpolated
{
    float3 Normal : NORMAL;
    float3 WorldPos : POSITION;
    float4 ViewPos : SV_Position;
};

Interpolated main( float3 pos : POSITION, float3 normal : NORMAL )
{
    Interpolated vsout;
    vsout.WorldPos = (float3) mul(float4(pos, 1), model);
    vsout.ViewPos = mul(float4(pos, 1), modelviewproj);
    vsout.Normal = mul(normal, (float3x3)model);
	return vsout;
}