#version 150 core

uniform ivec2 viewport;
uniform sampler2DArray source;

in vec3 g_uv;

out vec4 fragColor;

float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

void main()
{
	vec4 sum = texture(source, g_uv) * weight[0];
	for(int i=1; i<3; i++) {
		sum += texture(source, vec3(g_uv.x, g_uv.y + offset[i]/viewport.y, g_uv.z)) * weight[i];
		sum += texture(source, vec3(g_uv.x, g_uv.y - offset[i]/viewport.y, g_uv.z)) * weight[i];
	}
	fragColor = sum;
}
