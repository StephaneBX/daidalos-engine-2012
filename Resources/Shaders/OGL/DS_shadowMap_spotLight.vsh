#version 130

in vec3 vertexPosition0;

uniform mat4 worldMatrix;
uniform mat4 viewProjMatrix;

invariant gl_Position;
smooth out vec2 depth;

void main()
{
	vec4 worldVertex = worldMatrix * vec4(vertexPosition0, 1.0f);
	
	gl_Position = viewProjMatrix * worldVertex;
	depth = gl_Position.zw;
}
