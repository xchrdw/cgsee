#version 150 core

float linstep(float low, float high, float v)
{
	return clamp( (v - low)/(high - low), 0.0, 1.0);
}

float vsm(
	vec4 worldCoord,
	mat4 lightBiasedViewProjections[8],
	in sampler2DArray shadowmaps,
	int layer)
{
	float shadow = 1.0;
	vec4 shadowCoords = lightBiasedViewProjections[layer] * worldCoord;
	shadowCoords.xyz /= shadowCoords.w;
	
	if( shadowCoords.x < 0.0 ) return 1.0;
	if( shadowCoords.y < 0.0 ) return 1.0;
	if( shadowCoords.x > 1.0 ) return 1.0;
	if( shadowCoords.y > 1.0 ) return 1.0;
	
	if( shadowCoords.w > 0.0 )
	{
		float depth = shadowCoords.z;
		vec2 moments = texture(shadowmaps, vec3(shadowCoords.xy, layer)).xy;
		float p = smoothstep(depth - 0.01, depth, moments.x);
		float variance = max(moments.y - moments.x * moments.x, 0.0001);
		float d = depth - moments.x;
		float p_max = linstep(0.2, 1.0, variance / (d*d + variance));
		shadow = clamp(max(p, p_max), 0.0, 1.0);
	}
	return shadow;
}

int calculateLayer(
	vec4 viewCoord,
	int startIndex,
	float farSplits[4])
{
	int result = 0;
	for (int i = 0; i < 4; ++i)
		if(-viewCoord.z < farSplits[i])
		{
			return startIndex + i;
		}
		
	return result;
}

float shadowing(
	vec4 worldCoord, 									//shader
	vec4 viewCoord,										//shader
	mat4 lightBiasedViewProjections[8], 				//uniform
	in sampler2DArray shadowmaps, 						//uniform
	int startIndex,										//light
	float farSplits[4])									//uniform
{
	int index = calculateLayer(viewCoord, startIndex, farSplits);
	float result = vsm(worldCoord, lightBiasedViewProjections, shadowmaps, index);
	return result;
}