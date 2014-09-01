#version 150 core

uniform mat4 viewProjectionInverse;
uniform mat4 transform;

in vec3 a_vertex;
in vec3 a_normal;

void main(void) 
{
    vec3 normal = a_normal;
	gl_Position = viewProjectionInverse * transform * vec4(a_vertex, 1.0);
}