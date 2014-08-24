#version 150 core

uniform mat4 transform;
uniform mat4 inverseViewProjection;
uniform mat4 biasLightViewProjection[2];

in vec3 a_vertex;
in vec3 a_normal;

out vec3 normal;
out vec4 v_shadow[2];

void main()
{
	normal = a_normal;
	v_shadow[0] = biasLightViewProjection[0] * inverseViewProjection * transform * vec4(a_vertex, 1.0); 
	v_shadow[1] = biasLightViewProjection[1] * inverseViewProjection * transform * vec4(a_vertex, 1.0); 
	gl_Position = transform * vec4(a_vertex, 1.0);
}