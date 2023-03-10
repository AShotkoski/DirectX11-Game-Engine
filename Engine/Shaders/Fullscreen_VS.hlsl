struct VSOUT
{
    float2 tc : TEXCOORD;
    float4 pos : SV_Position;
};

VSOUT main( float2 pos : POSITION )
{
    VSOUT vout;
    vout.pos = float4(pos, 0, 1);
    vout.tc = float2((pos.x + 1) / 2.f, -(pos.y - 1) / 2.f);
	return vout;
}