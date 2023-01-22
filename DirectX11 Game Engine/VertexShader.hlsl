struct VSOut
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

VSOut main( float2 pos : POSITION, float3 color : COLOR )
{	
	VSOut ret;
	ret.pos = float4( pos, 1.f, 1.f );
	ret.color = color;
	return ret;
}