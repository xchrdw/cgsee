#version 150 core

uniform mat4 transform;
uniform mat4 viewProjectionInverse;

in vec3 a_vertex;
in vec3 a_normal;

out vec4 world;

void main(void) 
{
    vec3 normal = a_normal;
	world = vec4((viewProjectionInverse * transform * vec4(a_vertex, 1.0)).xyz, 1.0);
	//world.w = 1.0;
	gl_Position = transform * vec4(a_vertex, 1.0);
}