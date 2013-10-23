// #version 410
#version 150 core

in vec2 a_vertex;
// in vec3 a_normal;

uniform mat4 transformInverse;
uniform vec3 cameraposition;

// out vec3 normal;
// out vec3 position;

out vec2 v_uv;
out vec3 direction;

uniform vec2 antialiasingOffset;


void main(void) 
{
    vec2 vertexAntialiased = a_vertex + antialiasingOffset;
    vec4 worldPos = transformInverse * vec4(vertexAntialiased, -1.0, 1.0);
    direction = worldPos.xyz - cameraposition;
    
	v_uv = a_vertex.xy * 0.5 + 0.5;
	gl_Position = vec4(a_vertex, 0.0, 1.0);
}
