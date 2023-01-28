cbuffer cb
{
    matrix transformation;
};

struct interpolants
{
    float2 tex : TEXCOORD;
    float4 pos : SV_Position;
};

interpolants main(float3 pos : POSITION, float2 tc : TEXCOORD)
{
    interpolants vso;
    vso.tex = tc;
    vso.pos = mul(float4(pos, 1.f), transformation);
    return vso;
}