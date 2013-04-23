#version 330 core

out vec4 fragcolor;
in vec2 v_uv;

uniform sampler2D source;

void main()
{
	fragcolor = texture2D(source, v_uv);
}