#version 150

in vec3 a_vertex;

uniform mat4 transform;

out vec4 pos;

void main(void) {
    pos = transform * vec4(a_vertex, 1.0);
}
