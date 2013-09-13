//#version 330
#version 150 core

uniform mat4 transform;

in vec3 a_vertex;
in vec3 a_normal;

out vec3 normal;

void main(void) 
{
    normal = a_normal;
    gl_Position = transform * vec4(a_vertex, 1.0);
}
