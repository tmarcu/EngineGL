#version 430

in vec3 vPosition;
in vec3 vNormal;

uniform mat4 modelviewmatrix;
uniform mat4 viewmatrix;
uniform mat4 projectionmatrix;

uniform vec4 xscale = {1.0, 1.0, 1.0, 1.0};
uniform vec4 yscale;
uniform vec4 zscale;

out vec3 N;
out vec3 v;
out vec4 vcolor;

void main(void)
{
	gl_Position = projectionmatrix * modelviewmatrix * vec4(vPosition, 1.0);

	vec4 gvertex = vec4(vPosition, 1.0);

	gvertex.xyz *= xscale.xyz;
	v = vec3(modelviewmatrix * gvertex);
	N = normalize(modelviewmatrix * vec4(vNormal, 1.0)).xyz;

	vcolor = vec4(vNormal, 1.0);
}
