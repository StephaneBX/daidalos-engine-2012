uniform float4x4 worldMatrix;
uniform float4x4 rotationMatrix;
uniform float4x4 viewProjMatrix;

struct VS_INPUT {
	float3 position 	: POSITION0;
	float2 texcoord		: TEXCOORD0;
	float3 normal 		: NORMAL0;
	float3 tangent 		: NORMAL1;
	float3 binormal		: NORMAL2;
};

struct VS_OUTPUT {
	float4 position 	: POSITION;
	float2 texcoord		: TEXCOORD0;
	float3 normal 		: TEXCOORD1;
	float3 tangent 		: TEXCOORD2;
	float3 binormal		: TEXCOORD3;
	float  depth		: TEXCOORD4;
};

VS_OUTPUT main(VS_INPUT IN) {
	VS_OUTPUT OUT;
	float4 worldVertex = mul(float4(IN.position, 1.0f), worldMatrix);
	
	OUT.position 	= mul(worldVertex, viewProjMatrix);
	OUT.texcoord 	= IN.texcoord;
	OUT.normal 		= normalize(mul(IN.normal, rotationMatrix));
	OUT.tangent 	= normalize(mul(IN.tangent, rotationMatrix));
	OUT.binormal 	= normalize(mul(IN.binormal, rotationMatrix));
	OUT.depth    	= OUT.position.z;
	return OUT;
}
