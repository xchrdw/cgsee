#version 150 core


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec4 gl_FragCoord;
out vec4 fragColor;

in vec3 normal;
in vec3 position;

uniform vec3 cameraposition;

uniform vec3 lightdir;
uniform vec3 lightdir2;
uniform mat4 light;
uniform mat4 light2;
uniform vec4 lightambientglobal;
uniform mat4 material;

layout(std140) uniform LightInfo
{
	uint numPointLights;
	uint numSpotLights;
	struct
	{
		vec3 direction;
		vec3 intensity;
	} DirectionalLight;
} LightInfoData;


layout(std140) uniform PointLights
{
	struct
	{
		vec3 pos;
		vec3 intensity;
		vec3 falloff;
	} lights[12];
} PointLightBuffer;

vec4 phongLighting(vec3 n, vec3 v_pos, vec3 cameraposition, vec3 lightdir, vec3 lightdir2, mat4 light, mat4 light2, vec4 lightambientglobal, mat4 material);

void main()
{
	vec3 n = normal;
	vec3 dir = normalize(LightInfoData.DirectionalLight.direction);
	float n_dot_l = dot(n, dir);
	fragColor = n_dot_l * vec4(LightInfoData.DirectionalLight.intensity, 1.0);
	fragColor = vec4(PointLightBuffer.lights[0].intensity, 1.0);
	
	//fragColor=phongLighting(n, position, cameraposition, lightdir, lightdir2, light, light2, lightambientglobal, material);
}
