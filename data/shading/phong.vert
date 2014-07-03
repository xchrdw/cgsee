#version 150 core

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;
uniform mat4 view;

out vec3 view_normal;
out vec3 view_position;

void main(void) 
{
	view_normal = normalize(view * vec4(a_normal, 0.0)).xyz;
	view_position = vec3(view * vec4(a_vertex, 1.0));
	gl_Position = transform * vec4(a_vertex, 1.0);
}
