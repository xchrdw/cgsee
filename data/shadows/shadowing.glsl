#version 150 core

float linstep(float low, float high, float v)
{
	return clamp( (v - low)/(high - low), 0.0, 1.0);
}

float vsm(
	vec4 worldCoord,
	mat4 lightBiasedViewProjections[4],
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

float shadowing(
	vec4 worldCoord, 						//shader
	vec4 viewCoord,							//shader
	mat4 lightBiasedViewProjections[4], 	//uniform
	in sampler2DArray shadowmaps, 			//uniform
	int startIndex,							//light
	vec4 farSplits)							//light
{
	int index = 0;
	//if(-viewCoord.z < farSplits.x) index = startIndex;
	//else if (-viewCoord.z < farSplits.y) index = startIndex + 1;
	//else if (-viewCoord.z < farSplits.z) index = startIndex + 2;
	//else if (-viewCoord.z < farSplits.w) index = startIndex + 3;
	
	if(-viewCoord.z < farSplits.x) index = 0;
	else if (-viewCoord.z < farSplits.y) index = 1;
	else if (-viewCoord.z < farSplits.z) index = 2;
	else if (-viewCoord.z < farSplits.w) index = 3;

	float result = vsm(worldCoord, lightBiasedViewProjections, shadowmaps, index);
	return result;
}
