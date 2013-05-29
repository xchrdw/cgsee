#version 330


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec3 a_vertex;
in vec3 a_normal;
out vec4 color;

uniform mat4 transform;
uniform mat4 view;

uniform float znear;
uniform float zfar;

uniform vec3 cameraposition;
uniform vec3 lightdir;
uniform vec3 lightdir2;
uniform vec3 attCoeff;
uniform vec3 attCoeff2;

uniform mat4 light;
uniform mat4 light2;
uniform mat4 material;
uniform float lightshininess;
uniform vec4 lightambientglobal;

vec4 phongLighting(vec3 n, vec3 v_pos, vec3 cameraposition, vec3 lightdir, vec3 lightdir2, mat4 light, mat4 light2, vec4 lightambientglobal, mat4 material);

void main()
{
	gl_Position = transform * vec4(a_vertex, 1.0);
	vec3 n = normalize(a_normal);
	vec3 position = vec3(view * vec4(a_vertex, 1.0));

	color=phongLighting(n, position, cameraposition, lightdir, lightdir2, light, light2, lightambientglobal, material);

}
