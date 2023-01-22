float4 main(float3 Color : COLOR) : SV_TARGET
{
	return float4(Color, 1.0f);
}