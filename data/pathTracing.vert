#version 150 core

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;
uniform mat4 view;

out vec3 normal;
out vec3 position;
out vec3 objCoord;

void main(void) 
{
    normal = normalize(a_normal);
    position = (view * vec4(a_vertex, 1.0)).xyz;
    objCoord = a_vertex.xyz;
    gl_Position = transform * vec4(a_vertex, 1.0);
}