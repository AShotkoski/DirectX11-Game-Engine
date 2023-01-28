Texture2D tex;

SamplerState splr;

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
    return tex.Sample(splr, tc);
}