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

vec4 BlinnPhong(vec3 n, vec3 v, vec3 l, vec4 light_color, vec4 diff_color, vec4 spec_color, float spec_power, float attenuation);
vec4 BlinnPhongDirectional(vec3 n, vec3 v, vec3 l, vec4 light_color, vec4 diff_color, vec4 spec_color, float spec_power);

void main()
{
	vec3 n = normal;
	vec3 v = cameraposition - position;
	vec3 l = normalize(LightInfoData.DirectionalLight.direction);
	vec4 light_color = vec4(LightInfoData.instensity, 1.0);
//	fragColor = BlinnPhongDirectional(n, v, l, light_color, vec4(1.0, 1.0, 1.0, 1.0) vec4(1.0, 1.0, 1.0, 1.0), 16.f);
	fragColor = vec4(1.0);
}
