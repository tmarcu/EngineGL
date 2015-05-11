#version 430

in vec3 vPosition;
in vec3 vNormal;

uniform mat4 modelviewmatrix;
uniform mat4 viewmatrix;
uniform mat4 projectionmatrix;
uniform mat4 normalmatrix;

out vec3 vnorm;
out vec3 lightpos;
out vec4 vcolor;

vec3 lightsource = vec3(1.0, .50, 1.0);

void main(void)
{
	gl_Position = projectionmatrix * modelviewmatrix * vec4(vPosition, 1.0);

	lightpos = -(modelviewmatrix * vec4(lightsource, 1.0)).xyz;
	vnorm = normalize(normalmatrix * vec4(vNormal, 1.0)).xyz;

	vcolor = vec4(1.0, 1.0, 1.0, 1.0);
}
