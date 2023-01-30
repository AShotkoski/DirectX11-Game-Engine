cbuffer cb
{
    matrix model;
    matrix modelviewproj;
};

struct Interpolated
{
    float4 pos : SV_Position;
    float3 n : NORMAL;
};

Interpolated main( float3 pos : POSITION, float3 normal : NORMAL )
{
    Interpolated vsout;
    vsout.pos = float4(pos, 1);
    vsout.n = normal;
	return vsout;
}