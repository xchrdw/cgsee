#version 150 core

uniform sampler2D selectedBuffer;

out vec4 fragcolor;
in vec2 v_uv;

void main()
{
	fragcolor = texture(selectedBuffer, v_uv);
}
