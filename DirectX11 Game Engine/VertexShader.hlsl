struct VSOut
{
	float4 color : Color;
	float4 pos : SV_POSITION;
};

VSOut main( float4 pos : Position, float4 col : Color)
{	
	VSOut v;
	v.color = col;
	v.pos = pos;
	return v;
}