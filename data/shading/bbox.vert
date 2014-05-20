#version 150 core


in vec3 a_vertex;

uniform mat4 transform;


void main(void)
{
    gl_Position = transform * vec4(a_vertex, 1.0);
}
