#version 430

layout(location = 0) out vec4 fragcolor;

in vec3 N;
in vec3 v;
in vec4 vcolor;

void main(void)
{
	fragcolor = vcolor;
}