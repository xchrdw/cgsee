#version 150 core
uniform sampler2D shadowmap;
uniform sampler2D shadowmap2;
uniform sampler2DArray shadowmap3D;
uniform float znear;
uniform float zfar;
uniform vec4 farDistances;

in vec4 v_shadow[4];
in vec4 v_viewCoord;
in vec3 normal;

out vec4 fragColor;

float linearize(float depth)
{
	return - znear * depth / (zfar * depth - zfar - znear * depth);
}

float linstep(float low, float high, float v)
{
	return clamp( (v - low)/(high - low), 0.0, 1.0);
}

float vsm()
{
	int index = 4;
	float shadow = 1.0;
	if(-v_viewCoord.z < farDistances.x) index = 0;
	else if (-v_viewCoord.z < farDistances.y) index = 1;
	else if (-v_viewCoord.z < farDistances.z) index = 2;
	else if (-v_viewCoord.z < farDistances.w) index = 3;

	vec3 shadowCoords  = v_shadow[index].xyz / v_shadow[index].w;

	if( v_shadow[index].w > 0.0 )
	{
		float depth = shadowCoords.z;
		vec2 moments = texture(shadowmap3D, vec3(shadowCoords.xy, index)).xy;
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
	float shadow = vsm();
	fragColor = vec4(vec3(shadow), 1.0);
}