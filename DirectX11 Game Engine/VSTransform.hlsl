cbuffer cb
{
    matrix model;
    matrix view;
    matrix proj;
};

struct VSOut
{
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION)
{
    VSOut vout;
    vout.pos = mul(float4(pos, 1.f), model);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    return vout;
}