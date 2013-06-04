#version 150

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;

out vec3 o_normal;

void main(void) {
    o_normal = vec3(transform * vec4(a_normal, 0.0));
    gl_Position = transform * vec4(a_vertex, 1.0);
}
