#version 330

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;
uniform mat4 projection;

uniform float znear;
uniform float zfar;

out vec3 normal;

void main(void) 
{
    normal = mat3(transform) * a_normal;
	gl_Position = transform * vec4(a_vertex, 1.0);
}