#version 150 core

#define NUM_POINT_LIGHTS 12
#define NUM_SPOT_LIGHTS 12
#define NUM_DIRECTIONAL_LIGHTS 4

in vec4 gl_FragCoord;
out vec4 fragColor;

in vec3 view_normal;
in vec3 view_position;

uniform mat4 view;
uniform vec3 cameraposition;

layout(std140) uniform LightInfoBuffer
{
	uint numPointLights;
	uint numSpotLights;
	uint numDirectionalLights;
} LightInfo;


layout(std140) uniform PointLightBuffer
{
	struct
	{
		vec4 pos;
		vec4 intensity;
		vec3 falloff; // x: constant, y: linear, z: quadratic
		float radius;
	} lights[NUM_POINT_LIGHTS];
} PointLights;

layout(std140) uniform SpotLightBuffer
{
	struct
	{
		vec4 pos;
		vec4 direction;
		vec4 intensity;
		vec3 falloff; // x: constant, y: linear, z: quadratic
		float range;
		float conePower;
	} lights[NUM_SPOT_LIGHTS];
} SpotLights;

layout(std140) uniform DirectionalLightBuffer
{
    struct
    {
	vec4 direction;
	vec4 intensity;
    } lights[NUM_DIRECTIONAL_LIGHTS];
} DirectionalLights;

// Blinn-Phong for any light with attenuation
vec4 BlinnPhong(vec3 n, vec3 v, vec3 l, vec4 light_intensity, vec4 diff_color, vec4 spec_color, float spec_power, float attenuation)
{
    vec3 h = normalize(v + l);
    float n_dot_l = max(dot(n,l), 0);
    float n_dot_h = max(dot(n,h), 0);

    // Diffuse component
    vec4 diff = diff_color * n_dot_l;
    vec4 spec = spec_color * pow(n_dot_h, spec_power) * n_dot_l;

    vec4 intensity_out = (diff + spec) * light_intensity * vec4(attenuation);
    return intensity_out;
}

void main()
{
    uint numPointL = LightInfo.numPointLights;
    uint numSpotL = LightInfo.numSpotLights;
    uint numDirL= LightInfo.numDirectionalLights;

    vec3 n = normalize(view_normal);
    vec3 v = normalize(-view_position);
    vec3 l;
    vec4 light_intensity;
    vec4 intensity_out = vec4(0.0);
    vec4 diff_color = vec4(1.0);

    for (uint t = 0; t < numDirL; t++)
    {
	l = normalize(view * DirectionalLights.lights[t].direction).xyz;
	light_intensity = DirectionalLights.lights[t].intensity;
	intensity_out += BlinnPhong(n, v, l, light_intensity, diff_color, diff_color, 16.0, 1.0); 
    }

    for (uint t = 0; t < numPointL; t++)
    {
	// l: direction to the light from the surface
	l = (view * PointLights.lights[t].pos).xyz - view_position;
	light_intensity = PointLights.lights[t].intensity;
	float dist = length(l);
	l = normalize(l);
	float att = 1 / (PointLights.lights[t].falloff.x + PointLights.lights[t].falloff.y * dist + PointLights.lights[t].falloff.z * dist * dist);
	intensity_out += BlinnPhong(n, v, l, light_intensity, diff_color, diff_color, 64.0, att);
    }

    for (uint t = 0; t < numSpotL; t++)
    {
	// l: direction to the light from the surface
	// s: direction of the spotlight
	light_intensity.xyz = SpotLights.lights[t].intensity.xyz;
	l = (view * SpotLights.lights[t].pos).xyz - view_position;
	float dist = length(l);
	l = normalize(l);
	vec3 s = normalize(view * SpotLights.lights[t].direction).xyz;
	float l_dot_s = max(dot(l, -s), 0.0); // rhoi - cosine(angle)
	float spot_falloff = pow(l_dot_s, SpotLights.lights[t].conePower * 4);
	float att = spot_falloff / (SpotLights.lights[t].falloff.x + SpotLights.lights[t].falloff.y * dist + SpotLights.lights[t].falloff.z * dist * dist);
	intensity_out += BlinnPhong(n, v, l, light_intensity, diff_color, diff_color, 16.0, att);
    }

    fragColor = intensity_out;
    fragColor.w = 1.0;
}
