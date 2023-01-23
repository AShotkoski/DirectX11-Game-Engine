struct VSOut
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer cb
{
	matrix transformation;
};

VSOut main( float4 pos : POSITION, float3 color : COLOR )
{	
	VSOut ret;
	ret.pos = mul(pos, transformation);
	ret.color = color;
	return ret;
}