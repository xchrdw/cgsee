#version 150 core

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;
uniform mat4 view;

out vec3 _normal;
out vec4 _position;

void main(void) 
{
    _normal = normalize(a_normal);
    _position = transform * vec4(a_vertex, 1.0);
    //position = vec3(view * vec4(a_vertex, 1.0)); why define both??
}