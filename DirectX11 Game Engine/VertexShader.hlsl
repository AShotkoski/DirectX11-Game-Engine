struct VSOut
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer cb
{
	row_major matrix transformation;
};

VSOut main( float2 pos : POSITION, float3 color : COLOR )
{	
	VSOut ret;
	ret.pos = mul(float4( pos, 1.f, 1.f ), transformation);
	ret.color = color;
	return ret;
}