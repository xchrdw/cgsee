#version 150 core

out vec4 fragcolor;
in vec2 v_uv;

uniform sampler2D source;
uniform sampler2D shadows;
uniform sampler2D ssao;


void main()
{
    // if texture(source, v_uv).w == 0, we paint a bounding box,
    // therefore we don't apply the shadowing to that fragment.

    fragcolor = mix(
        texture(source, v_uv) * min(texture(shadows, v_uv)+0.2, texture(ssao, v_uv)),
        texture(source, v_uv),
        step(texture(source, v_uv).w, 0.0)
        );
}
