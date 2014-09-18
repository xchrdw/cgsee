#version 150 core

uniform sampler2D selectedBuffer;
uniform sampler2D lighting;

out vec4 fragcolor;
in vec2 v_uv;

void main()
{
	vec4 color = texture(selectedBuffer, v_uv);
	float intensity = texture(lighting, v_uv).x;
	fragcolor = vec4(color.xyz * intensity, 1.0);
}
