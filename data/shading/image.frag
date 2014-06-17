#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;

in vec2 v_uv;

uniform sampler2D image;

void main()
{
	fragColor = texture(image, v_uv);
}
