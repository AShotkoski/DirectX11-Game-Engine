cbuffer LightCBuf
{
    float3 lightposition;   
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attenConst;
    float attenLin;
    float attenQuad;
};

cbuffer ObjectCBuf : register(b1)
{
    float3 MaterialColor;
};

struct PSIn // VSOUT
{
    float3 Normal : NORMAL;
    float3 WorldPos : POSITION;
    float4 ViewPos : SV_Position;
};

float4 main(PSIn psin) : SV_TARGET
{
    const float3 vertToL = lightposition - psin.WorldPos;
    const float distVertToL = length(vertToL);
    const float3 dirVertToL = vertToL / distVertToL;
    
    const float attenuation = 1.f / (pow(distVertToL, 2) * attenQuad
                              + distVertToL * attenLin
                              + attenConst);
    
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation
                                * max(0, dot(dirVertToL, psin.Normal));

    return float4(saturate(diffuse + ambient) * MaterialColor, 1);
}