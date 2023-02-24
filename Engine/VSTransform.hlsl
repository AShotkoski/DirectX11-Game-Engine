cbuffer cb
{
    matrix model;
    matrix InvView;
    matrix modelViewProj;
};

struct VSOut
{
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION)
{
    VSOut vout;
    vout.pos = mul(float4(pos, 1.f), modelViewProj);
    return vout;
}