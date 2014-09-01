#version 150 core

uniform sampler2DArray shadowmaps;
uniform mat4 lightBiasedViewProjections[16];

in vec4 gl_FragCoord;
in vec4 world;
out vec4 fragColor;

float shadowing(
	vec4 worldCoord,
	mat4 lightBiasedViewProjections[16],
	in sampler2DArray shadowmaps,
	int startIndex);

void main()
{
	fragColor = vec4(vec3(shadowing(world, lightBiasedViewProjections, shadowmaps, 0)), 1.0);
}