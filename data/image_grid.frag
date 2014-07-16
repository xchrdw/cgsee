#version 150 core

in vec4 gl_FragCoord;
in float v_linePos;
in float v_scale;

out vec4 fragColor;

uniform float pixelWidth;

void main()
{
	float gray = int(mod(v_linePos * v_scale, 2));
	fragColor = vec4(vec3(gray), smoothstep(40,60,pixelWidth));
}
