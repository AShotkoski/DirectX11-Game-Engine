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
    float specularIntensity;
    float specularPower;
};

struct PSIn // VSOUT
{
    float3 Normal : NORMAL;
    float3 WorldPos : POSITION;
    float3 EyePos : EYEPOS;
    float2 texcoord : TEXCOORD;
    float4 ViewPos : SV_Position;
};

SamplerState splr;
Texture2D tex;

float4 main(PSIn psin) : SV_TARGET
{
    // Normalize normal
    psin.Normal = normalize(psin.Normal);
    
    float3 pxToLight = lightposition - psin.WorldPos;
    const float distPxToLight = length(pxToLight);
    
    // Calculate the attenuation ( inverse square law )
    const float attenuation = 1.f / (pow(distPxToLight, 2) * attenQuad
                              + distPxToLight * attenLin
                              + attenConst);
    
    // Get the diffuse lighting
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation
                                * max(0, dot(normalize(pxToLight), psin.Normal));
    // Calc specular ( blinn-phong )
    const float3 pxToEye = normalize(psin.EyePos - psin.WorldPos);
    pxToLight = normalize(pxToLight);
    const float3 halfWay = normalize(pxToEye + pxToLight);
    // Calc cos of the angle between the surface normal and the halfway
    float cosBetween = max(dot(psin.Normal, halfWay), 0.0);
    float3 specular;
    if (dot(psin.Normal, pxToLight) <= 0)
    {
        specular = 0;
    }
    else
    {
        specular = specularIntensity * pow(cosBetween, specularPower) * attenuation;
    }
    return float4(saturate(diffuse + ambient + specular) * (float3) tex.Sample(splr, psin.texcoord), 1);
}