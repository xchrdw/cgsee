#version 150 core

uniform float timef;

out vec4 fragcolor;
in vec2 v_uv;

void main()
{
    fragcolor = vec4(gl_FragCoord.x / 200.0, gl_FragCoord.y / 200.0, 0.0, 1.0);
}
