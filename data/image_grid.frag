#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform float zoom;
uniform vec2 pan;
uniform vec2 aspect;

void main()
{
	fragColor = vec4(1,1,1,1);
}
