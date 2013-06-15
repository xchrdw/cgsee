#version 150 core

in vec3 normal;
in vec3 position;

out vec4 fragColor;

uniform vec3 cameraposition;

void main()
{
	fragColor = vec4((position - cameraposition), 1.0);
    //fragColor = vec4(normal, 1.0);
}
