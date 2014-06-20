#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;

in vec3 view_normal;
in vec3 view_position;

uniform mat4 view;
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

layout(std140) uniform SpotLights
{
	struct
	{
		vec4 pos;
		vec4 direction;
		vec4 intensity;
		vec3 falloff;
		float conePower;
		float dist;
	} lights[12];
} SpotLightBuffer;

// Blinn-Phong for any light with attenuation
vec4 BlinnPhong(vec3 n, vec3 v, vec3 l, vec4 light_color, vec4 diff_color, vec4 spec_color, float spec_power, float attenuation)
{
    vec3 h = normalize(v + l);
    float n_dot_l = max(dot(n,l), 0);
    float n_dot_h = max(dot(n,h), 0);

    // Diffuse component
    vec4 diff = diff_color * n_dot_l;
    vec4 spec = spec_color * pow(n_dot_h, spec_power) * 0;

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
    vec4 spec = spec_color * pow(n_dot_h, spec_power) * 0.0;

    vec4 color_out = (diff + spec) * light_color;
    return color_out;
}

void main()
{
    uint numPointL = LightInfoData.numPointLights;
    uint numSpotL = LightInfoData.numSpotLights;

    vec3 n = normalize(view_normal);
    vec3 v = normalize(-view_position);
    vec3 l = normalize(view * vec4(LightInfoData.DirectionalLight.direction, 0.0)).xyz;
    vec4 diff_color = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 light_color = vec4(LightInfoData.DirectionalLight.intensity, 1.0);
    fragColor = BlinnPhongDirectional(n, v, l, light_color, diff_color, diff_color, 16.0);

    for (uint t = 0; t < numPointL; t++)
    {
	l = (view * vec4(PointLightBuffer.lights[t].pos.xyz,1.0)).xyz - view_position;
	light_color = PointLightBuffer.lights[t].intensity;
	float dist = length(l);
	l = normalize(l);
	float att = 1 / (PointLightBuffer.lights[t].falloff.x + PointLightBuffer.lights[t].falloff.y * dist + PointLightBuffer.lights[t].falloff.z * dist * dist);
	fragColor += BlinnPhong(n, v, l, light_color, diff_color, diff_color, 64.0, att);
    }

    for (uint t = 0; t < numSpotL; t++)
    {
	// l: direction to the light from the surface
	// s: direction of the spotlight
	light_color.xyz = SpotLightBuffer.lights[t].intensity.xyz;
	vec3 l = (view * vec4(SpotLightBuffer.lights[t].pos.xyz, 1.0)).xyz - view_position;
	float dist = length(l);
	l = normalize(l);
	vec3 s = normalize(view * vec4(SpotLightBuffer.lights[t].direction.xyz, 0.0)).xyz;
	float l_dot_s = max(dot(l,-s), 0.0); // rhoi - cosine(angle)
	float spot_falloff = pow(l_dot_s, SpotLightBuffer.lights[t].conePower * 4);
	float att = spot_falloff / (SpotLightBuffer.lights[t].falloff.x + SpotLightBuffer.lights[t].falloff.y * dist + SpotLightBuffer.lights[t].falloff.z * dist * dist);
	fragColor += BlinnPhong(n, v, l, light_color, diff_color, diff_color, 16.0, att);
    }

    fragColor.w = 1.0;

}
