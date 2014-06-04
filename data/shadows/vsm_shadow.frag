#version 150 core
//TODO: Layout setzen auf 0 f�r shadowmap
uniform sampler2D shadowmap;
uniform float znear;
uniform float zfar;

in vec4 v_shadow;

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
	vec3 shadowCoords = v_shadow.xyz / v_shadow.w;
	if( v_shadow.w > 0.0 )
	{
		float depth = linearize(shadowCoords.z);
		vec2 moments = texture(shadowmap, shadowCoords.xy).xy;
		float p = smoothstep(depth - 0.01, depth, moments.x);
		float variance = max(moments.y - moments.x * moments.x, 0.0);
		float d = depth - moments.x;
		float p_max = linstep(0.2, 1.0, variance / (d*d + variance));
		p_max = variance / (d*d + variance);
		shadow = clamp(max(p, p_max), 0.0, 1.0);
	}
	return shadow;
}

void main()
{
	fragColor = vec4(vec3(vsm()), 1.0);
}