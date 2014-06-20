#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform float pixelWidth;

void main()
{
	fragColor = mix( vec4(1,1,1,0), vec4(1,1,1,1), smoothstep(49,55,pixelWidth) );
}
