#version 150 core

out vec4 fragcolor;
in vec2 v_uv;

uniform sampler2D source;
uniform sampler2D effect;


void main()
{
	fragcolor = texture(source, v_uv); // * texture(effect, v_uv);
}