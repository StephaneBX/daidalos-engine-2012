#define PI 3.14159265f

uniform float4x4 viewProjMatrix;
uniform float4x4 billboardMatrix;
uniform float2 textureAtlasSize;

struct VS_INPUT {
	float3 position 					: POSITION0; // Vertex position (non translated)
	float2 texcoord 					: TEXCOORD0; // Texture coordinate (non translated)
	float4 instPositionRotation	: POSITION1; // Translation and rotation factor
	float4 instColor    				: COLOR0; // Color
	float2 instSizeId					: POSITION2; // Rotation value, Size modification, Lifetime [0 - 1] and textureId
};

struct VS_OUTPUT {
	float4 position 		: POSITION; // Screen space coordinate
	float2 texcoord 		: TEXCOORD0; // Texture coordinate
	float4 color	  		: COLOR0; // Color
};

VS_OUTPUT main(VS_INPUT IN) {
	VS_OUTPUT OUT;
	
	// Position data
	float4 v = float4( IN.position.x,
						IN.position.y,
					   IN.position.z,
					   1.0f );
					   
	// Rotation data
	float4x4 rotationMatrix = float4x4(	cos(IN.instPositionRotation.w * PI / 180.0f),  sin(IN.instPositionRotation.w * PI / 180.0f),0,0,
								-sin(IN.instPositionRotation.w * PI / 180.0f), cos(IN.instPositionRotation.w * PI / 180.0f),0,0,
								0,0,1,0,
								0,0,0,1);
	
	// Modify the position by assigning the rotation
	v = mul( v, rotationMatrix );
	
	// Assign the size
    v.xy *= IN.instSizeId.x;
	
	// Billboard
	v = mul( v, billboardMatrix );
	
	// Translation
	v.xyz += IN.instPositionRotation.xyz;
	
	OUT.position = mul(v, viewProjMatrix);
	
	// Texture coordinate translation
	float2 step = 1.0f / textureAtlasSize.xy;
	OUT.texcoord.x = (IN.texcoord.x * step.x) + (step.x * (IN.instSizeId.y % (int)textureAtlasSize.x));
	OUT.texcoord.y = (IN.texcoord.y * step.y) + (step.y * (IN.instSizeId.y / textureAtlasSize.x));

	OUT.color = IN.instColor;
	
	return OUT;
}
