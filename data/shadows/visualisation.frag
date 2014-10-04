#version 150 core

uniform sampler2DArray shadowmaps;
uniform mat4 lightBiasedViewProjections[8];
uniform float farSplits[4];

in vec4 gl_FragCoord;
in vec4 worldCoord;
in vec4 viewCoord;
out vec4 fragColor;

float shadowing(
	vec4 worldCoord,
	vec4 viewCoord,
	mat4 lightBiasedViewProjections[8],
	in sampler2DArray shadowmaps,
	int startIndex,
	float farSplits[4]);

void main()
{
	float shadow1 = shadowing(worldCoord,viewCoord, lightBiasedViewProjections, shadowmaps, 0, farSplits);
	float shadow2 = shadowing(worldCoord,viewCoord, lightBiasedViewProjections, shadowmaps, 4, farSplits);
	fragColor = vec4(vec3((shadow1 + shadow2) * 0.5), 1.0);
}