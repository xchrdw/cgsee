#version 150 core

uniform mat4 transform;
uniform mat4 viewProjectionInverse;
uniform mat4 view;

in vec3 a_vertex;
in vec3 a_normal;

out vec4 worldCoord;
out vec4 viewCoord;

void main(void) 
{
    vec3 normal = a_normal;
	worldCoord = vec4((viewProjectionInverse * transform * vec4(a_vertex, 1.0)).xyz, 1.0);
	viewCoord = view * worldCoord;

	gl_Position = transform * vec4(a_vertex, 1.0);
}