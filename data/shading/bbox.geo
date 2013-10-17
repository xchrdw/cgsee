#version 150 core


// in vec4 _pos[2];
// in vec3 b_vertex;

layout(lines) in;
layout(line_strip, max_vertices = 128) out;

uniform mat4 transform;
uniform ivec2 viewport;

out vec2 v_uv;
out float depth;
flat out int visible;


void main(void)
{
    vec2 screen_pos[2];
    screen_pos[0] = gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;
    screen_pos[1] = gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;

    screen_pos[0] = screen_pos[0]/2.0 + vec2(0.5);
    screen_pos[1] = screen_pos[1]/2.0 + vec2(0.5);

    vec2 diff_vec = viewport * (screen_pos[1] - screen_pos[0]);
    vec2 seg_vec = normalize(diff_vec) * 20;
    int segments = int(abs(length(diff_vec))/abs(length(seg_vec)));


    for( int i = 0; i < segments; ++i )
    {
        vec2 vertex;

        vertex = screen_pos[0] + (i * seg_vec);
        v_uv = vertex * 0.5 + vec2(0.5);
        depth = mix(
            gl_in[0].gl_Position.z/gl_in[0].gl_Position.w,
            gl_in[1].gl_Position.z/gl_in[1].gl_Position.w,
            length((i * seg_vec))/length(diff_vec));

        visible = int(mod(i, 2));
        gl_Position = mix(
            gl_in[0].gl_Position,
            gl_in[1].gl_Position,
            length((i * seg_vec))/length(diff_vec));
        EmitVertex();

        vertex = screen_pos[0] + ((i+1) * seg_vec);
        v_uv = vertex * 0.5 + vec2(0.5);
        depth = mix(
            gl_in[0].gl_Position.z/gl_in[0].gl_Position.w,
            gl_in[1].gl_Position.z/gl_in[1].gl_Position.w,
            length(((i+1) * seg_vec))/length(diff_vec));

        visible = int(mod(i, 2));
        gl_Position = mix(
            gl_in[0].gl_Position,
            gl_in[1].gl_Position,
            length(((i+1) * seg_vec))/length(diff_vec));
        EmitVertex();

        EndPrimitive();
    }
}
