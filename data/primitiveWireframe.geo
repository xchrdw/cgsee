#version 150

#define M_PI 3.1415926535897932384626433832795

in vec3 v_normal[];
out float edge_dist[3];

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;


void main(void) {
    gl_Position = gl_in[0].gl_Position; EmitVertex();
    gl_Position = gl_in[1].gl_Position; EmitVertex();
    gl_Position = gl_in[2].gl_Position; EmitVertex();
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    EndPrimitive();
}
