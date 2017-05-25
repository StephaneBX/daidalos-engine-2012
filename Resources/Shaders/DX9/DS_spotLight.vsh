struct VS_INPUT {
	float3 position 	: POSITION0;
	float2 texcoord		: TEXCOORD0;
	float3 viewDistance	: TEXCOORD1;
};

struct VS_OUTPUT {
	float4 position 	: POSITION;
	float2 texcoord		: TEXCOORD0;
	float3 viewDistance	: TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT IN) {
	VS_OUTPUT OUT;

	OUT.position = float4(IN.position, 1.0f);
	OUT.texcoord = IN.texcoord;
	OUT.viewDistance = IN.viewDistance;
	
	return OUT;
}
