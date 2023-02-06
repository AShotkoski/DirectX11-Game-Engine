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
    float3 EyePos : EYEPOS;
    float4 ViewPos : SV_Position;
};

static const float specularIntensity = 1.0f;
static const float shiny = 1.f;

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
    float cosBetween = saturate(dot(psin.Normal, halfWay));

    const float3 specular = specularIntensity * pow(cosBetween, shiny) * attenuation;
    
    return float4(saturate(diffuse + ambient + specular) * MaterialColor, 1);
}