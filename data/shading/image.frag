#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;

in vec2 v_uv;

uniform sampler2D image;
uniform float zoom;
uniform vec2 pan;
uniform vec2 aspect;

void main()
{
	fragColor = texture(image, (v_uv/zoom+pan)/aspect);
}
