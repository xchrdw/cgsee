#version 150 core
uniform sampler2D shadowmap;
uniform sampler2D shadowmap2;
uniform sampler2DArray shadowmap3D;
uniform float znear;
uniform float zfar;

in vec4 v_shadow[2];
in vec3 normal;

out vec4 fragColor;

float linstep(float low, float high, float v)
{
	return clamp( (v - low)/(high - low), 0.0, 1.0);
}

float linearize(float depth) {
	return - znear * depth / (zfar * depth - zfar - znear * depth);
}

float vsm()
{
	float shadow = 1.0;
	vec3 shadowCoords = v_shadow[0].xyz / v_shadow[0].w;
	if( v_shadow[0].w > 0.0 )
	{
		float depth = linearize(shadowCoords.z);
		vec2 moments = texture(shadowmap, shadowCoords.xy).xy;
		float p = smoothstep(depth - 0.01, depth, moments.x);
		float variance = max(moments.y - moments.x * moments.x, 0.0002);
		float d = depth - moments.x;
		float p_max = linstep(0.2, 1.0, variance / (d*d + variance));
		p_max = variance / (d*d + variance);
		shadow = clamp(max(p, p_max), 0.0, 1.0);
	}
	return shadow;
}

float vsm2()
{
	float shadow = 1.0;
	vec3 shadowCoords = v_shadow[1].xyz / v_shadow[1].w;
	if( v_shadow[1].w > 0.0 )
	{
		float depth = linearize(shadowCoords.z);
		vec2 moments = texture(shadowmap2, shadowCoords.xy).xy;
		float p = smoothstep(depth - 0.01, depth, moments.x);
		float variance = max(moments.y - moments.x * moments.x, 0.0002);
		float d = depth - moments.x;
		float p_max = linstep(0.2, 1.0, variance / (d*d + variance));
		p_max = variance / (d*d + variance);
		shadow = clamp(max(p, p_max), 0.0, 1.0);
	}
	return shadow;
}

float vsm3D(int layer)
{
	float shadow = 1.0;
	vec3 shadowCoords = v_shadow[layer].xyz / v_shadow[layer].w;
	if( v_shadow[layer].w > 0.0 )
	{
		float depth = linearize(shadowCoords.z);
		vec2 moments = texture(shadowmap3D, vec3(shadowCoords.x, shadowCoords.y, layer)).xy;
		float p = smoothstep(depth - 0.01, depth, moments.x);
		float variance = max(moments.y - moments.x * moments.x, 0.0002);
		float d = depth - moments.x;
		float p_max = linstep(0.2, 1.0, variance / (d*d + variance));
		p_max = variance / (d*d + variance);
		shadow = clamp(max(p, p_max), 0.0, 1.0);
	}
	return shadow;
}

void main()
{
	//float shadow = (vsm2() + vsm()) / 2.0;
	float shadow = (vsm3D(0) + vsm3D(1)) / 2.0;
	fragColor = vec4(vec3(shadow), 1.0);
}