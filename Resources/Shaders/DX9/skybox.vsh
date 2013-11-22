uniform float3 skyboxPosition;
uniform float skyboxSize;
uniform float4x4 viewProjMatrix;

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
	
	OUT.position = mul(float4((IN.position * skyboxSize) + skyboxPosition, 1), viewProjMatrix);
	OUT.texcoord = IN.texcoord;
	
	return OUT;
}
