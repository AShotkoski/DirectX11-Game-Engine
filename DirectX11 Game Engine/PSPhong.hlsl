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

float4 main(float3 normal : NORMAL, float3 worldPos : POSITION) : SV_TARGET
{
    const float3 vertToL = lightposition - worldPos;
    const float distVertToL = length(vertToL);
    const float3 dirVertToL = vertToL / distVertToL;
    
    const float attenuation = 1.f / (pow(distVertToL, 2) * attenQuad
                              + distVertToL * attenLin
                              + attenConst);
    
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation
                                * max(0, dot(dirVertToL, normal));

    return float4(saturate(diffuse + ambient) * MaterialColor, 1);
}