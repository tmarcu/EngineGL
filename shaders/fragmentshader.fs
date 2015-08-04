#version 430

layout(location = 0) out vec4 fragcolor;

in vec3 vnorm;
in vec3 lightpos;
in vec4 vcolor;

void main(void)
{
	vec4 diffuse = vec4(max(dot(vnorm, normalize(lightpos)), 0.0));

	fragcolor = diffuse * normalize(vcolor);
}