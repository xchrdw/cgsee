#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

vec3 o_normal[3];

void main(void) {
    for(int i = 0; i < gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(normalize(o_normal[i])*3, 1.0);
        EmitVertex();

        EndPrimitive();
    }
}

