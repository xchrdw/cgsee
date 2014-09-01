#version 150 core

uniform sampler2D selectedBuffer;
uniform sampler2D lighting;
uniform mat4 lightBiasedViewProjections[16];
uniform mat4 viewProjectionInverse;

out vec4 fragcolor;
in vec2 v_uv;

float shadowing(
	vec4 worldCoord,
	mat4 lightBiasedViewProjections[16],
	in sampler2DArray shadowmaps,
	int startIndex);

void main()
{
	vec4 color = texture(selectedBuffer, v_uv);
	float intensity = texture(lighting, v_uv).x;
	fragcolor = vec4(color.xyz * intensity, 1.0);
}
