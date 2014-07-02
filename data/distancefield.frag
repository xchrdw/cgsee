#version 150 core

in vec2 v_uv;
in int v_char;

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform sampler2D distancefield;
uniform float pixelWidth;

void main()
{
	fragColor = vec4(v_uv,0,smoothstep(40,60,pixelWidth));
	fragColor = vec4(float(v_char)/16, 0, 0, 1);
}
