uniform float3 translation;
uniform float4x4 projMatrix;

struct VS_INPUT
{
	float3 position	: POSITION0;
	float2 texcoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position	: POSITION;
	float2 texcoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT;
	
	OUT.position = mul(float4(IN.position + translation, 1), projMatrix);
	OUT.texcoord = IN.texcoord;
	
	return OUT;
}
