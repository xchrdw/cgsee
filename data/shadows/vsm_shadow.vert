#version 150 core

uniform mat4 transform;
uniform mat4 inverseViewProjection;
uniform mat4 biasLightViewProjection;

in vec3 a_vertex;
in vec3 a_normal;

out vec3 normal;
out vec4 v_shadow;

void main()
{
	normal = a_normal;
	v_shadow = biasLightViewProjection * inverseViewProjection * transform * vec4(a_vertex, 1.0); 
	gl_Position = transform * vec4(a_vertex, 1.0);
}