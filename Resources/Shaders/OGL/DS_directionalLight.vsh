#version 130

in vec3 vertexPosition0;
in vec2 vertexTexcoord0;
in vec3 vertexTexcoord1;

invariant gl_Position;
smooth out vec2 out_texcoord;
smooth out vec3 out_viewDistance;

void main()
{
	gl_Position = vec4(vertexPosition0, 1);
	out_texcoord = vertexTexcoord0;
	out_viewDistance = vertexTexcoord1;
}
