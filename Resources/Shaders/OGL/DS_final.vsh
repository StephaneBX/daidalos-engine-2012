#version 130

in vec3 vertexPosition0;
in vec2 vertexTexcoord0;

invariant gl_Position;

smooth out vec2 out_texcoord;

void main()
{
	gl_Position = vec4(vertexPosition0, 1.0);
	out_texcoord = vertexTexcoord0;
}
