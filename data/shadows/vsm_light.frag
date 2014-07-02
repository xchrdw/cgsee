#version 150 core

uniform float znear;
uniform float zfar;

flat in int g_layer;

out vec4 fragColor;


float linearize(float depth) 
{
	return - znear * depth / (zfar * depth - zfar - znear * depth);
}

void main()
{
	float depth = linearize(gl_FragCoord.z);
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	
	fragColor = vec4(depth, depth*depth + 0.25 * (dx*dx + dy*dy), 0.0, 1.0);
}