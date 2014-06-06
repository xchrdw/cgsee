#version 150 core


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec4 gl_FragCoord;
out vec4 fragColor;

in vec3 normal;
in vec3 position;

uniform vec3 cameraposition;

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
		vec4 pos;
		vec4 intensity;
		vec4 falloff;
	} lights[12];
} PointLightBuffer;

// Blinn-Phong for any light with attenuation
vec4 BlinnPhong(vec3 n, vec3 v, vec3 l, vec4 light_color, vec4 diff_color, vec4 spec_color, float spec_power, float attenuation)
{
    vec3 h = normalize(v + l);
    float n_dot_l = max(dot(n,l), 0);
    float n_dot_h = max(dot(n,h), 0);

    // Diffuse component
    vec4 diff = diff_color * n_dot_l;
    vec4 spec = spec_color * pow(n_dot_h, spec_power);

    vec4 color_out = (diff + spec) * light_color * vec4(attenuation);
    return color_out;
}

// Blinn-Phong for any light without attenuation
vec4 BlinnPhongDirectional(vec3 n, vec3 v, vec3 l, vec4 light_color, vec4 diff_color, vec4 spec_color, float spec_power)
{
    vec3 h = normalize(v + l);
    float n_dot_l = max(dot(n,l), 0);
    float n_dot_h = max(dot(n,h), 0);

    // Diffuse component
    vec4 diff = diff_color * n_dot_l;
    vec4 spec = spec_color * pow(n_dot_h, spec_power);

    vec4 color_out = (diff + spec) * light_color;
    return color_out;
}

void main()
{
	vec3 n = normal;
	vec3 v = cameraposition - position;
	vec3 l = normalize(LightInfoData.DirectionalLight.direction);
	vec4 diff_color = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 light_color = vec4(LightInfoData.DirectionalLight.intensity, 1.0);
	fragColor = BlinnPhongDirectional(n, v, l, light_color, diff_color, diff_color, 16.0);

	for (uint t = 0; t < LightInfoData.numPointLights; t++)
	{
	    l = PointLightBuffer.lights[t].pos.xyz - position;
	    light_color = PointLightBuffer.lights[t].intensity;
	    float dist = length(l);
	    l = normalize(l);
	    float att = 1 / (PointLightBuffer.lights[t].falloff.x + PointLightBuffer.lights[t].falloff.y * dist + PointLightBuffer.lights[t].falloff.z * dist * dist);
	    fragColor += BlinnPhong(n, v, l, light_color, diff_color, diff_color, 16.0, att);
	}

	fragColor.w = 1.0;

}
