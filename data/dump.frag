#version 150 core

out vec4 fragcolor;
in vec2 v_uv;

void main()
{
    fragcolor = texture(0, v_uv);
}
