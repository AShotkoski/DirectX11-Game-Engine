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
};

struct PSIn // VSOUT
{
    float3 Normal : NORMAL;
    float3 WorldPos : POSITION;
    float3 EyePos : EYEPOS;
    float2 texcoord : TEXCOORD;
    float4 ViewPos : SV_Position;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
};

SamplerState splr;
Texture2D tex;
Texture2D norm_map : register(t1);

float4 main(PSIn psin) : SV_TARGET
{
   // psin.Normal = CalculateNormalFromMap(
   //                                     norm_map.Sample(splr, psin.texcoord).xyz,
   //                                     normalize(psin.Normal), 
   //                                     normalize(psin.Tangent),
   //                                     normalize(psin.Bitangent));
    psin.Normal = CalculateNormalFromMap_HD(
                                        norm_map.Sample(splr, psin.texcoord).xyz,
                                        normalize(psin.Normal), 
                                        psin.Tangent);
    
    const float distPxToLight = length(lightposition - psin.WorldPos);
    float3 pxToLight = normalize(lightposition - psin.WorldPos);
    const float3 pxToEye = normalize(psin.EyePos - psin.WorldPos);
    
    const float attenuation = Attenuate(distPxToLight, attenQuad, attenLin, attenConst);
    
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity * attenuation,
                            pxToLight, psin.Normal);
    
    float3 specularColor = specularIntensity * attenuation;
    float3 specular = Speculate_BlinnPhong(specularColor,
                                            specularPower, pxToEye, pxToLight, psin.Normal);

    return float4(saturate(diffuse + ambient + specular) * (float3)tex.Sample(splr, psin.texcoord), 1);
}