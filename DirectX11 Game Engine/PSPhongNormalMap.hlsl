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
    // Calculate normal from map
    float3x3 tan_to_obj_space =
    {
        normalize(psin.Tangent),
        normalize(psin.Bitangent),
        normalize(psin.Normal)
    };
    float3 tan_normal = norm_map.Sample(splr, psin.texcoord);
    tan_normal.x = 2.f * tan_normal.x - 1.f;
    tan_normal.y = 2.f * tan_normal.y - 1.f;
    tan_normal.z = 2.f * tan_normal.z - 1.f;
    psin.Normal = normalize(mul(tan_normal, tan_to_obj_space));
    
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