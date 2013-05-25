#version 330

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;
uniform mat4 view;

uniform float znear;
uniform float zfar;

out vec3 normal;

void main(void) 
{
	//vec4 temp= normalize(view*vec4(a_normal,0.0));
	//normal=temp.xyz;
	gl_Position = transform * vec4(a_vertex, 1.0);
}