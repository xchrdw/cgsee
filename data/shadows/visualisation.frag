#version 150 core

uniform sampler2DArray shadowmaps;
uniform mat4 lightBiasedViewProjections[4];

in vec4 gl_FragCoord;
in vec4 worldCoord;
in vec4 viewCoord;
out vec4 fragColor;

float shadowing(
	vec4 worldCoord,
	vec4 viewCoord,
	mat4 lightBiasedViewProjections[4],
	in sampler2DArray shadowmaps,
	int startIndex,
	vec4 farSplits);

void main()
{
	fragColor = vec4(vec3(shadowing(worldCoord,viewCoord, lightBiasedViewProjections, 
									shadowmaps, 0, vec4(4.0, 7.5, 12.0, 20.0))), 1.0);
}