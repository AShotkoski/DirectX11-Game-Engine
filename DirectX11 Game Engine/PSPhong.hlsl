cbuffer LightCBuf
{
    float3 lightposition;
};

static const float3 matColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.21f, 0.21f, 0.21f };
static const float3 diffuseColor = { 1.f, 1.f, 1.f };
static const float diffuseIntensity = 40.f;
static const float attenConst = 1.f;
static const float attenLin = 1.f;
static const float attenQuad = 2.3f;

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

    return float4(saturate(diffuse + ambient), 1);
}