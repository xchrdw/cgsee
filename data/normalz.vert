#version 150 core

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;

uniform float znear;
uniform float zfar;

out vec3 v_normal;

void main(void) 
{
    v_normal = a_normal;
	gl_Position = transform * vec4(a_vertex, 1.0);
}