#version 150 core

float linstep(float low, float high, float v)
{
	return clamp( (v - low)/(high - low), 0.0, 1.0);
}

float vsm(
	vec4 worldCoord,
	mat4 lightBiasedViewProjections[16],
	sampler2DArray shadowmaps,
	int layer)
{
	float shadow = 1.0;
	vec4 shadowCoords = lightBiasedViewProjections[layer] * worldCoord;
	shadowCoords.xyz /= shadowCoords.w;
	if( shadowCoords.w > 0.0 )
	{
		float depth = shadowCoords.z;
		vec2 moments = texture(shadowmaps, vec3(shadowCoords.xy, layer)).xy;
		float p = smoothstep(depth - 0.001, depth, moments.x);
		float variance = max(moments.y - moments.x * moments.x, -0.0001);
		float d = depth - moments.x;
		float p_max = linstep(0.2, 1.0, variance / (d*d + variance));
		shadow = clamp(max(p, p_max), 0.0, 1.0);
	}
	return shadow;
}

float shadowing(
	vec4 worldCoord, 						//shader
	mat4 lightBiasedViewProjections[16], 	//uniform
	in sampler2DArray shadowmaps, 			//uniform
	int startIndex)							//light
{
	float result = vsm(worldCoord, lightBiasedViewProjections, shadowmaps, startIndex);
	return result;
}