cbuffer cb
{
    float3 c[8];
};

float4 main(uint pid : SV_PrimitiveID ) : SV_TARGET
{
    return float4(c[(pid / 2) % 8], 1.0f);
}