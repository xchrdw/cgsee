#version 150 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 normal;
in vec3 _normal[3];

in vec4 _position[3];
out vec4 position;

void main(void) {

    normal=normalize( (_normal[0] + _normal[1] + _normal[2])/3 );

    position = _position[0];
    gl_Position=_position[0];
    EmitVertex();
    
    position = _position[1];
    gl_Position=_position[1];
    EmitVertex();

    position = _position[2];
    gl_Position=_position[2];
    EmitVertex();

    EndPrimitive();
}