#version 130

in vec3 vertexPosition0;
in vec2 vertexTexcoord0;
in vec3 vertexNormal0;

uniform mat4 worldMatrix;
uniform mat4 rotationMatrix;
uniform mat4 viewProjMatrix;

invariant gl_Position;
smooth out vec2 out_texcoord;
smooth out vec3 out_normal;
smooth out float depth;

void main()
{
	vec4 worldVertex = worldMatrix * vec4(vertexPosition0, 1.0f);
	
	gl_Position = viewProjMatrix * worldVertex;
	out_texcoord = vertexTexcoord0;
	
	out_normal = normalize((rotationMatrix * vec4(vertexNormal0, 1)).xyz);
	depth = gl_Position.z;
}
