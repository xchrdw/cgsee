#version 150 core

uniform mat4 transform;

in vec3 a_vertex;

void main()
{
	gl_Position = transform * vec4(a_vertex, 1.0);
}