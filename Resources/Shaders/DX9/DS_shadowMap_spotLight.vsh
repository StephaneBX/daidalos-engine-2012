uniform float4x4 worldMatrix;
uniform float4x4 viewProjMatrix;

struct VS_INPUT
{
	float3 position 	: POSITION0;
};

struct VS_OUTPUT
{
	float4 position 	: POSITION;
	float2 depth		: TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT;
	
	float4 worldVertex = mul(float4(IN.position, 1.0f), worldMatrix);
	OUT.position = mul(worldVertex, viewProjMatrix);
	OUT.depth = OUT.position.zw;
	
	return OUT;
}
