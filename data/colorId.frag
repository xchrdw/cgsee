#version 150

out vec4 fragColor;

uniform vec4 u_id;

void main(void) {
    fragColor = u_id;
}

