#version 430

in vec3 vPosition;
in vec3 vNormal;

uniform mat4 modelviewmatrix;
uniform mat4 viewmatrix;
uniform mat4 projectionmatrix;

out vec3 N;
out vec3 v;
varying vec4 vcolor;

void main(void)
{
	gl_Position = projectionmatrix * modelviewmatrix * vec4(vPosition, 1.0);

	v = vec3(modelviewmatrix * vec4(vPosition, 1.0));
	N = normalize(modelviewmatrix * vec4(vNormal, 1.0)).xyz;

	vcolor = vec4(vNormal, 1.0);
}
