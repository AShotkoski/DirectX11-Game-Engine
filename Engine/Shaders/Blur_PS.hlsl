struct VSOUT
{
    float2 tc : TEXCOORD;
};

Texture2D tex;
SamplerState splr;

float4 main(VSOUT psin) : SV_TARGET
{
    uint width;
    uint height;
    tex.GetDimensions(width, height);
    float aspectRatio = (float) width / (float) height;
    float4 sampled = tex.Sample(splr, psin.tc);
    float4 sampledinv = 0.5f - sampled;
    psin.tc = psin.tc - 0.5f;
    psin.tc.x = psin.tc.x * aspectRatio;
    psin.tc = psin.tc * 2.f;
    float distFromCenterSQ = psin.tc.x * psin.tc.x + psin.tc.y * psin.tc.y; // (0 ~ 0.5)
    float distadjusted = distFromCenterSQ * 35.f;
    
    return distadjusted > 0.5f ? sampled : sampledinv;
}