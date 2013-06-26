#version 330

in vec3 a_vertex;
in vec3 a_normal;

uniform mat4 transform;
uniform mat4 LightSourceTransform;
uniform mat4 invCameraTransform;
uniform mat4 biasMatrix;

uniform float znear;
uniform float zfar;

out vec3 normal;
out vec4 shadowCoord;


void main(void) 
{
    normal = a_normal;
	gl_Position = transform * vec4(a_vertex, 1.0);
	shadowCoord = biasMatrix * LightSourceTransform * invCameraTransform * transform * vec4(a_vertex, 1.0);
}
