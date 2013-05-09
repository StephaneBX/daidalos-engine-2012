#version 130

in vec3 vertexPosition0;
in vec2 vertexTexcoord0;

uniform mat4 projMatrix;

invariant gl_Position;
smooth out vec2 out_texcoord;

void main()
{
	gl_Position = projMatrix * vec4(vertexPosition0, 1);
	out_texcoord = vertexTexcoord0;
}
