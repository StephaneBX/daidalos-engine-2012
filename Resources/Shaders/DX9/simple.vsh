uniform float4x4 viewProjMatrix;

struct VS_INPUT
{
	float3 position	: POSITION0;
};

struct VS_OUTPUT
{
	float4 position	: POSITION;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT;
	float4 worldVertex = float4(IN.position, 1.0f);
	
	OUT.position = mul(worldVertex, viewProjMatrix);
	
	return OUT;
}
