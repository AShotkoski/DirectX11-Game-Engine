struct VSOUT
{
    float2 tc : TEXCOORD;
};

Texture2D tex;
SamplerState splr;

float4 main(VSOUT psin) : SV_TARGET
{
    return 1.f - tex.Sample(splr, psin.tc);
}