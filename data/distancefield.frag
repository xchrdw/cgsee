#version 150 core

in vec2 v_uv;

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform sampler2D distancefield;

void main()
{
	fragColor = vec4(v_uv,0,1);
}
