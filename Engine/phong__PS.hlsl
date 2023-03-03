#include "Operations.hlsli"

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
    float specularIntensity;
    float specularPower;
    float3 matColor;
};

struct PSIn // VSOUT
{
    float3 Normal : NORMAL;
    float3 WorldPos : POSITION;
    float3 EyePos : EYEPOS;
    float4 ViewPos : SV_Position;
};

float4 main(PSIn psin) : SV_TARGET
{
    // Normalize normal
    psin.Normal = normalize(psin.Normal);
    
    const float distPxToLight = length(lightposition - psin.WorldPos);
    float3 pxToLight = normalize(lightposition - psin.WorldPos);
    const float3 pxToEye = normalize(psin.EyePos - psin.WorldPos);
    
    const float attenuation = Attenuate(distPxToLight, attenQuad, attenLin, attenConst);
    
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity * attenuation,
                            pxToLight, psin.Normal);
    
    float3 specularColor = specularIntensity * attenuation;
    float3 specular = Speculate_BlinnPhong(specularColor,
                                            specularPower, pxToEye, pxToLight, psin.Normal);

    return float4(saturate(diffuse + ambient + specular) * matColor, 1);
}