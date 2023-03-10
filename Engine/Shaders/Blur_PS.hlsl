struct VSOUT
{
    float2 tc : TEXCOORD;
};

Texture2D tex;
SamplerState splr;

float4 main(VSOUT psin) : SV_TARGET
{
    float4 sampled = tex.Sample(splr, psin.tc);
    return (0.6f - sampled);
}