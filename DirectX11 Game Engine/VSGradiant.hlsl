struct VSOut
{
	float4 color : Color;
	float4 pos : SV_POSITION;
};

cbuffer cb
{
	matrix transformation;
};

VSOut main( float3 pos : Position, float4 col : Color)
{	
	VSOut v;
	v.color = col;
	v.pos = mul(float4(pos,1.f), transformation);
	return v;
}