#version 150 core


// in vec3 position;
// in vec3 b_vertex;
in vec2 v_uv;
in float depth;
flat in int visible;

out vec4 fragcolor;

uniform sampler2D depthTexture;
uniform ivec2 viewport;


void main(void)
{
    fragcolor = mix(
        vec4(0.0, 1.0 * visible, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        step(depth/2 + 0.5, texture(depthTexture, v_uv).r)
        );
}
