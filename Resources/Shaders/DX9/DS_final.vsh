struct VS_INPUT
{
	float3 position 	: POSITION0;
	float2 texcoord		: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position 	: POSITION;
	float2 texcoord		: TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	OUT.position = float4(IN.position,1);
	OUT.texcoord = IN.texcoord;
	
	return OUT;
}
