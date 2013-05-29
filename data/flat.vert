#version 330

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;
uniform mat4 view;

uniform float znear;
uniform float zfar;

out vec3 _normal;
out vec3 position;

void main(void) 
{
	_normal = normalize(a_normal);
	gl_Position = transform * vec4(a_vertex, 1.0);
	position = vec3(view * vec4(a_vertex, 1.0));
}